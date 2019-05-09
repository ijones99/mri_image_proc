// --------- March 2017, Ian L Jones ---------
//
//GetWc
//    ->SetInputI0(DOUBLE VEC VOL, I0);
//    ->SetInputPc(DOUBLE VOL, Pc);
//    ->SetInputI1(DOUBLE VEC VOL, I1);
//    ->SetInputA(DOUBLE VOL, A);
//    ->SetWcScale(DOUBLE VAL, WC_SCALE);
//    ->SetSMax(DOUBLE VAL, WC_S_MAX);
//    ->GetOutput(DOUBLE VOL);
//

#ifndef __GetWc_txx
#define __GetWc_txx

#include "GetWc.h"

#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
//#include "ImageFilterMultipleOutputsDifferentType.h" // add C functions
#include "itkImportImageFilter.h"

namespace itk
{

template<  typename TI0, typename TPc, typename TI1,typename TA, typename TOut>
GetWc<TI0, TPc, TI1, TA, TOut>::GetWc()
{

    this->SetNumberOfRequiredInputs(4);

    this->SetNthOutput( 0, this->MakeOutput(0) );

}

template<  typename TI0, typename TPc, typename TI1,typename TA, typename TOut>
void GetWc<TI0, TPc, TI1, TA, TOut>::SetInputI0(const TI0* i0)
{
    this->SetNthInput(0, const_cast<TI0*>(i0));
}

template<  typename TI0, typename TPc, typename TI1,typename TA, typename TOut>
void GetWc<TI0, TPc, TI1, TA, TOut>::SetInputPc(const TPc* pc)
{
    this->SetNthInput(1, const_cast<TPc*>(pc));
}
template<  typename TI0, typename TPc, typename TI1,typename TA, typename TOut>
void GetWc<TI0, TPc, TI1, TA, TOut>::SetInputI1(const TI1* i1)
{
    this->SetNthInput(2, const_cast<TI1*>(i1));
}

template<  typename TI0, typename TPc, typename TI1,typename TA, typename TOut>
void GetWc<TI0, TPc, TI1, TA, TOut>::SetInputA(const TA* a)
{
    this->SetNthInput(3, const_cast<TA*>(a));
}
//------------------------------------------------
template<  typename TI0, typename TPc, typename TI1,typename TA, typename TOut>
typename TI0::ConstPointer GetWc<TI0, TPc, TI1, TA, TOut>::GetInputI0()
{
    return static_cast< const TI0 * >
           ( this->ProcessObject::GetInput(0) );
}
template<  typename TI0, typename TPc, typename TI1,typename TA, typename TOut>
typename TPc::ConstPointer GetWc<TI0, TPc, TI1, TA, TOut>::GetInputPc()
{
    return static_cast< const TPc * >
           ( this->ProcessObject::GetInput(1) );
}
template<  typename TI0, typename TPc, typename TI1,typename TA, typename TOut>
typename TI1::ConstPointer GetWc<TI0, TPc, TI1, TA, TOut>::GetInputI1()
{
    return static_cast< const TI1 * >
           ( this->ProcessObject::GetInput(2) );
}

template<  typename TI0, typename TPc, typename TI1,typename TA, typename TOut>
typename TA::ConstPointer GetWc<TI0, TPc, TI1, TA, TOut>::GetInputA()
{
    return static_cast< const TA * >
           ( this->ProcessObject::GetInput(3) );
}

template<  typename TI0, typename TPc, typename TI1,typename TA, typename TOut>
void GetWc<TI0, TPc, TI1, TA, TOut>::GenerateData()
{
    /*

    wc = 0.5*(abs(I0) + pc.*abs(I1)./A);
    fc = 0.5*(abs(I0) - pc.*abs(I1)./A);

    wc = abs(wc);
    fc = abs(fc);

    wc(isinf(wc)) = NaN;
    fc(isinf(fc)) = NaN;

    wc(wc < 0) = NaN;
    fc(fc < 0) = NaN;

    wc(wc > Smax) = NaN;
    fc(fc > Smax) = NaN;

     */
    const double SCALE = this->GetWcScale();
    const double S_MAX = this->GetSMax();

    typedef itk::Image< double, 2 >       PixelTypeVec;
    typedef itk::Image< PixelTypeVec, 2 >    ImageTypeVec;

    typename TI0::ConstPointer i0 = this->GetInputI0();
    typename TPc::ConstPointer pc = this->GetInputPc();
    typename TI1::ConstPointer i1 = this->GetInputI1();
    typename TA::ConstPointer a = this->GetInputA();

    typename TOut::Pointer output = this->GetOutput();


    // allocate space for output
    typename TPc::RegionType region = pc->GetLargestPossibleRegion();
    typename TPc::SizeType regionSize = region.GetSize();
    output->SetRegions(regionSize);
    output->Allocate();

    for (int m=0; m<=regionSize[2]-1; m++)
    {
        for (int j=0; j<=regionSize[1]-1; j++)
        {
            for (int i=0; i<=regionSize[0]-1; i++)
            {




                // get value of I0
                typename TI0::IndexType Pixel1 ;
                Pixel1[0] = i;
                Pixel1[1] = j;
                Pixel1[2] = m;
                typename TI0::PixelType valI0;
                valI0 = i0->GetPixel(Pixel1);
                // get value of PC
                typename TPc::IndexType Pixel2 ;
                Pixel2[0] = i;
                Pixel2[1] = j;
                Pixel2[2] = m;
                typename TPc::PixelType valPc;
                valPc = pc->GetPixel(Pixel2);
                // get value of I1
                typename TI1::IndexType Pixel3 ;
                Pixel3[0] = i;
                Pixel3[1] = j;
                Pixel3[2] = m;
                typename TI1::PixelType valI1;
                valI1 = i1->GetPixel(Pixel3);
                // get value of A
                typename TA::IndexType Pixel4 ;
                Pixel4[0] = i;
                Pixel4[1] = j;
                Pixel4[2] = m;
                typename TA::PixelType valA;
                valA = a->GetPixel(Pixel4);


                if (valI0[0]==0)
                    valI0[0] = 0;
                if (valI0[1]==0)
                    valI0[1] = 0;
                if (valI1[0]==0)
                    valI1[0] = 0;
                if (valI1[1]==0)
                    valI1[1] = 0;
                if (valPc==0)
                    valPc = 0;
                if (valA==0)
                    valA = 0;




                //wc = 0.5*(abs(I0) + pc.*abs(I1)./A);
                double absI0 = (std::sqrt(std::pow(valI0[0],2)+std::pow(valI0[1],2)));

                double absI1 = (std::sqrt(std::pow(valI1[0],2)+std::pow(valI1[1],2)));
                double cOut = (0.5*(absI0 + valPc*absI1/valA));

                /*if (i==148-1 && j == 193-1 && m == 21-1)
                {
                        std::cout << "\n\n\nWC INFO>>> "<<  valI0[0] << ", " << valI0[1] << ", "
                        << valI1[0]<< ", " << valI1[1] << ", " << valPc  << ", " << valA << ", " << cOut << std::endl;

                }*/

                // wc = abs(wc);
                //std::cout << "cOut " << cOut << "\n";
                //cOut = std::abs(cOut);
                // wc(isinf(wc)) = NaN;

                /*std::cout << "double valI0_0 = " << valI0[0] << "; double valI0_1 = " << valI0[1] <<   " ;double absI0 = "
                 << absI0<<  " ;double valPc = " << valPc<< " ; double valA = " << valA<<
                 " ;double valI1_0 = " << valI1[0]<<
                  " ;double valI1_1 = " << valI1[1]<<
                 " ;double cOut = " << cOut<< std::endl;*/
                if(isinf(cOut))
                {
                    cOut = std::numeric_limits<unsigned char >::quiet_NaN();
                }
                // wc(wc < 0) = NaN;
                else if (cOut < 0 )
                {
                    cOut = std::numeric_limits<unsigned char >::quiet_NaN();
                }

                // wc(wc > Smax) = NaN;
                else if (cOut > S_MAX )
                {
                    cOut = std::numeric_limits<unsigned char >::quiet_NaN();
                }




                output->SetPixel(Pixel4, cOut);
            }
        }
    }


}
template<  typename TI0, typename TPc, typename TI1,typename TA, typename TOut> // why is TImage a class when there is only one type and a typename
// when there are multiple image types?
DataObject::Pointer GetWc<TI0, TPc, TI1, TA, TOut>::
MakeOutput(unsigned int idx)
{
    DataObject::Pointer output;

    switch ( idx )
    {
    case 0:
        output = ( TOut::New() ).GetPointer();
        break;
    default:
        std::cerr << "No output " << idx << std::endl;
        output = NULL;
        break;
    }
    return output.GetPointer();
}

template<  typename TI0, typename TPc, typename TI1,typename TA, typename TOut>
TOut* GetWc<TI0, TPc, TI1, TA, TOut>::GetOutput()
{
    return dynamic_cast< TOut * >(this->ProcessObject::GetOutput(0) );
}


}// end namespace


#endif
