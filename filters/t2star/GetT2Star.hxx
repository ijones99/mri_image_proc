
// --------- March 2017, Ian L Jones ---------
//
//GetT2Star
//    ->SetInputA(DOUBLE VOL, A);
//    ->SetT2StarMax(DOUBLE VAL, T2STAR_MAX);
//    ->SetTeVal1(DOUBLE VAL, teVal1);
//    ->SetTeVal2(DOUBLE VAL, teVal2);
//    ->SetTeVal3(DOUBLE VAL, teVal3);
//    ->GetOutput(DOUBLE VOL);
//

#ifndef __GetT2Star_txx
#define __GetT2Star_txx

#include "GetT2Star.h"

#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

namespace itk
{

template< typename TInputA, typename TOutputT2Star>
GetT2Star<TInputA, TOutputT2Star>::GetT2Star()
{

    this->SetNumberOfRequiredInputs(1);

    this->SetNthOutput( 0, this->MakeOutput(0) );

}

template< typename TInputA, typename TOutputT2Star>
void GetT2Star<TInputA, TOutputT2Star>::SetInputA(const TInputA* a)
{
    this->SetNthInput(0, const_cast<TInputA*>(a));
}

template< typename TInputA, typename TOutputT2Star >
typename TInputA::ConstPointer GetT2Star<TInputA, TOutputT2Star>::GetInputA()
{
    return static_cast< const TInputA * >
           ( this->ProcessObject::GetInput(0) );
}

template< typename TInputA, typename TOutputT2Star>
void GetT2Star<TInputA, TOutputT2Star>::GenerateData()
{

    // std::cout << ">>>>>>>>>>>>>>>>>>>this far..."<< std::endl;
    typename TInputA::ConstPointer a = this->GetInput();
    typename TInputA::RegionType region = a->GetLargestPossibleRegion();
    typename TInputA::SizeType regionSize = region.GetSize();

    typename TOutputT2Star::Pointer output = this->GetOutput();

    typedef itk::Image<double, 2>   ImageType;


    // Setup output 1
    output->SetRegions(regionSize);
    output->Allocate();

    /*
    deltaTE = (TE(3)-TE(1))/2;
    T2star = abs(deltaTE./log(A));
    T2star(isinf(T2star)) = NaN;
    T2star(T2star > T2star_max) = NaN;
    */
    const double T2STAR_MAX = this->GetT2StarMax();
    const double teVal1 = this->GetTeVal1();
    const double teVal2 = this->GetTeVal2();
    const double teVal3 = this->GetTeVal3();
    const double teVal[] = {teVal1,  teVal2,  teVal3};

    double deltaTE = (teVal[2] - teVal[0])/2;

    double t2Star = 0;

    for (int i=0; i<regionSize[0]; i++)

        for (int j=0; j<regionSize[1]; j++)
        {

            for (int m=0; m<regionSize[2]; m++)
            {
                // get phase & mag values
                typename TInputA::IndexType Pixel1 ;
                Pixel1[0] = i;
                Pixel1[1] = j;
                Pixel1[2] = m;
                typename ImageType::PixelType aVal = a->GetPixel(Pixel1);

                // COMPUTE: T2star = abs(deltaTE./log(A));
                double aValDouble = static_cast<double>(aVal);
                //std::cout << "aVal>>>>" << aVal << std::endl;
                double logAVal = log(aValDouble);
                double absDeltLog = (std::abs(deltaTE/logAVal));
                //std::cout << "absDeltLog----->>>>" << absDeltLog<< std::endl;

                if(isinf(absDeltLog))
                {
                    // T2star(isinf(T2star)) = NaN;
                    t2Star = std::numeric_limits<double >::quiet_NaN();
                }
                else if( absDeltLog > T2STAR_MAX)
                {
                    //  T2star(T2star > T2star_max) = NaN;
                    t2Star = std::numeric_limits<double >::quiet_NaN();
                }
                else
                {
                    t2Star = static_cast<double>(absDeltLog);
                    //std::cout << "t2Star>>>>  " << t2Star << std::endl;

                }

                output->SetPixel(Pixel1, t2Star);
            }
        }
}



template< typename TInputA, typename TOutputT2Star>
DataObject::Pointer GetT2Star<TInputA, TOutputT2Star>::
MakeOutput(unsigned int idx)
{
    DataObject::Pointer output;

    switch ( idx )
    {
    case 0:
        output = ( TOutputT2Star::New() ).GetPointer();
        break;
    default:
        std::cerr << "No output " << idx << std::endl;
        output = NULL;
        break;
    }
    return output.GetPointer();
}

template< typename TInputA, typename TOutputT2Star>
TOutputT2Star* GetT2Star<TInputA, TOutputT2Star>::GetOutput()
{
    return dynamic_cast< TOutputT2Star * >(this->ProcessObject::GetOutput(0) );
}


}// end namespace


#endif
