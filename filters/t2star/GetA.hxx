// --------- March 2017, Ian L Jones ---------
//
//GetA
//    ->SetInputI2(DOUBLE VEC VOL, I2);
//    ->SetInputI0(DOUBLE VEC VOL, I0);
//    ->SetTeVal1(DOUBLE VAL, teVal1);
//    ->SetTeVal2(DOUBLE VAL, teVal2);
//    ->SetTeVal3(DOUBLE VAL, teVal3);
//    ->GetOutput(DOUBLE VOL );
//

#ifndef __GetA_txx
#define __GetA_txx
#include "GetA.h"
#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "math.h"

namespace itk
{

template< typename TInputI2, typename TInputI0, typename TOutput>
GetA<TInputI2, TInputI0, TOutput>::GetA()
{

    this->SetNumberOfRequiredInputs(2);

    this->SetNthOutput( 0, this->MakeOutput(0) );

}

template< typename TInputI2, typename TInputI0, typename TOutput>
void GetA<TInputI2, TInputI0, TOutput>::SetInputI2(const TInputI2* imageI2)
{
    this->SetNthInput(0, const_cast<TInputI2*>(imageI2));
}
template< typename TInputI2, typename TInputI0, typename TOutput>
void GetA<TInputI2, TInputI0, TOutput>::SetInputI0(const TInputI0* imageI0)
{
    this->SetNthInput(1, const_cast<TInputI0*>(imageI0));
}

template< typename TInputI2, typename TInputI0, typename TOutput >
typename TInputI2::ConstPointer GetA<TInputI2, TInputI0, TOutput>::GetInputI2()
{
    return static_cast< const TInputI2 * >
           ( this->ProcessObject::GetInput(0) );
}

template< typename TInputI2, typename TInputI0, typename TOutput >
typename TInputI0::ConstPointer GetA<TInputI2, TInputI0, TOutput>::GetInputI0()
{
    return static_cast< const TInputI0 * >
           ( this->ProcessObject::GetInput(1) );
}


template< typename TInputI2, typename TInputI0, typename TOutput>
void GetA<TInputI2, TInputI0, TOutput>::GenerateData()
{

    typename TInputI2::ConstPointer imageI2 = this->GetInputI2();
    typename TInputI0::ConstPointer imageI0 = this->GetInputI0();
    typename TInputI0::RegionType region = imageI0->GetLargestPossibleRegion();
    typename TInputI0::SizeType regionSize = region.GetSize();

    typename TOutput::Pointer output = this->GetOutput();

    output->SetRegions(regionSize);
    output->Allocate();
    double aOut = 0;

    const double teVal1 = this->GetTeVal1();
    const double teVal2 = this->GetTeVal2();
    const double teVal3 = this->GetTeVal3();

    const double teVal[] = {teVal1,  teVal2,  teVal3};


    for (int i=0; i<regionSize[0]; i++)
    {
        for (int j=0; j<regionSize[1]; j++)
        {

            for (int m=0; m<regionSize[2]; m++)
            {
                // get pixel value
                typename TInputI2::IndexType Pixel1 ;
                Pixel1[0] = i;
                Pixel1[1] = j;
                Pixel1[2] = m;
                // get pixel value
                typename TInputI2::PixelType valI2 = imageI2->GetPixel(Pixel1);
                typename TInputI0::PixelType valI0 = imageI0->GetPixel(Pixel1);

                // deltaTE = (TE(3)-TE(1))/2;  % in ms
                // A = sqrt(abs(I2)./abs(I0));
                // A(isinf(A)) = NaN;


                double deltaTE  = (teVal[2]- teVal[0])/2;
                double absI2 = sqrt(pow(valI2[0],2)+pow(valI2[1],2));
                double absI0 = sqrt(pow(valI0[0],2)+pow(valI0[1],2));
                double A = sqrt(absI2/absI0);


                if(isinf(A))

                {
                    aOut = std::numeric_limits<double >::quiet_NaN();

                }
                else
                {
                    aOut = A;
                }
                output->SetPixel(Pixel1, aOut);
            }

        }
    }
}



template< typename TInputI2, typename TInputI0, typename TOutput>
DataObject::Pointer GetA<TInputI2, TInputI0, TOutput>::
MakeOutput(unsigned int idx)
{
    DataObject::Pointer output;

    switch ( idx )
    {
    case 0:
        output = ( TOutput::New() ).GetPointer();
        break;
    default:
        std::cerr << "No output " << idx << std::endl;
        output = NULL;
        break;
    }
    return output.GetPointer();
}

template< typename TInputI2, typename TInputI0, typename TOutput>
TOutput* GetA<TInputI2, TInputI0, TOutput>::GetOutput()
{
    return dynamic_cast< TOutput * >(this->ProcessObject::GetOutput(0) );
}


}// end namespace



#endif
