#include "std.hpp"

namespace ReversibleLogic
{

Scheme RmGenerator::generate(const TruthTable& inputTable, ostream& outputLog)
{
    TruthTable table = inputTable;
    RmSpectra spectra = RmSpectraUtils::calculateRmSpectra(table);

    uint size = table.size();
    uint n = (uint)(log(size) / log(2));

    word mostSignificantBit = (word)1 << (n - 1);

    Scheme scheme;

    // 1) process first spectra row
    {
        word row = spectra.front();
        word mask = 1;

        while (mask <= row)
        {
            if (row & mask)
            {
                word targetMask = mask;

                scheme.push_front(ReverseElement(n, targetMask));
                applyTransformation(&table, targetMask);
            }

            mask <<= 1;
        }

        if (row)
            spectra = RmSpectraUtils::calculateRmSpectra(table);
    }

    // 2) process rest rows
    for (uint index = 0; index < size; ++index)
    {
        word row = spectra[index];
        if (RmSpectraUtils::isSpectraRowIdent(row, index))
            continue;

        bool isVariableRow = RmSpectraUtils::isVariableRow(index);
        if (isVariableRow)
        {
            if ((row & index) == 0)
            {
                word mask = mostSignificantBit;
                while ((row & mask) == 0 && mask)
                    mask >>= 1;

                assertd(mask && mask != index,
                    string("RmGenerator::generate(): failed to process variable row"));

                scheme.push_front(ReverseElement(n, index, mask));
                applyTransformation(&table, index, mask);
            }

            word mask = 1;
            while (mask <= row)
            {
                if (mask != index && (row & mask))
                {
                    scheme.push_front(ReverseElement(n, mask, index));
                    applyTransformation(&table, mask, index);
                }

                mask <<= 1;
            }
        }
        else
        {
            word controlMask = mostSignificantBit;
            while (controlMask)
            {
                if ((row & controlMask) != 0 &&
                    (index & controlMask) == 0)
                    break;

                controlMask >>= 1;
            }

            assertd(controlMask,
                string("RmGenerator::generate(): failed to process variable row"));

            deque<ReverseElement> elements;

            word mask = 1;
            while (mask <= row)
            {
                if (mask != controlMask && (row & mask))
                {
                    ReverseElement element(n, mask, controlMask);
                    elements.push_front(element);

                    scheme.push_front(element);
                    applyTransformation(&table, mask, controlMask);
                }

                mask <<= 1;
            }

            scheme.push_front(ReverseElement(n, controlMask, index));
            applyTransformation(&table, controlMask, index);

            // check if we need to apply elements to make previous rows canonical
            bool needApply = false;
            for (uint i = 0; i < size && !needApply; ++i)
            {
                if (i == index)
                    continue;

                if (spectra[i] & controlMask)
                    needApply = true;
            }

            if (needApply)
            {
                for (auto& element : elements)
                {
                    scheme.push_front(element);
                    applyTransformation(&table, element.getTargetMask(), element.getControlMask());
                }
            }
            
        }

        spectra = RmSpectraUtils::calculateRmSpectra(table);
    }

    return scheme;
}

void RmGenerator::applyTransformation(TruthTable* tablePtr, word targetMask, word controlMask /*= 0*/)
{
    assertd(tablePtr, string("RmGenerator::applyTransformation(): null ptr"));

    assertd(countNonZeroBits(targetMask) == 1 && (controlMask & targetMask) == 0,
        string("RmGenerator::applyTransformation(): invalid arguments"));

    TruthTable& table = *tablePtr;
    uint size = table.size();

    for (uint index = 0; index < size; ++index)
    {
        word value = table[index];
        if ((value & controlMask) == controlMask)
            table[index] = value ^ targetMask;
    }
}

} //namespace ReversibleLogic