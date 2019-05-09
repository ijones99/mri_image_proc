
// --------- March 2017, Ian L Jones ---------
//
//MagToComplex
//    ->SetInputImage1(DOUBLE VOL imageS1mag);
//    ->SetInputImage2(DOUBLE VOL imageS1PhaseRad);
//    ->GetOutput(DOUBLE VEC VOL);
//

#ifndef __MagToComplex_txx
#define __MagToComplex_txx

#include "MagToComplex.h"

#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

namespace itk
{

template< typename TInputImage1, typename TInputImage2, typename TOutputImage1>
MagToComplex<TInputImage1, TInputImage2, TOutputImage1>::MagToComplex()
{

    this->SetNumberOfRequiredInputs(2);

    this->SetNthOutput( 0, this->MakeOutput(0) );

}

template< typename TInputImage1, typename TInputImage2, typename TOutputImage1>
void MagToComplex<TInputImage1, TInputImage2, TOutputImage1>::SetInputImage1(const TInputImage1* image1)
{
    this->SetNthInput(0, const_cast<TInputImage1*>(image1));
}

template< typename TInputImage1, typename TInputImage2, typename TOutputImage1>
void MagToComplex<TInputImage1, TInputImage2, TOutputImage1>::SetInputImage2(const TInputImage2* image2)
{
    this->SetNthInput(1, const_cast<TInputImage2*>(image2));
}

template< typename TInputImage1, typename TInputImage2, typename TOutputImage1 >
typename TInputImage1::ConstPointer MagToComplex<TInputImage1, TInputImage2, TOutputImage1>::GetInput1()
{
    return static_cast< const TInputImage1 * >
           ( this->ProcessObject::GetInput(0) );
}

template< typename TInputImage1, typename TInputImage2, typename TOutputImage1 >
typename TInputImage2::ConstPointer MagToComplex<TInputImage1, TInputImage2, TOutputImage1>::GetInput2()
{
    return static_cast< const TInputImage2 * >
           ( this->ProcessObject::GetInput(1) );
}

template< typename TInputImage1, typename TInputImage2, typename TOutputImage1>
void MagToComplex<TInputImage1, TInputImage2, TOutputImage1>::GenerateData()
{

    // std::cout << ">>>>>>>>>>>>>>>>>>>this far..."<< std::endl;
    typename TInputImage1::ConstPointer Mag = this->GetInput1();
    typename TInputImage1::RegionType regionMag = Mag->GetLargestPossibleRegion();
    typename TInputImage1::SizeType regionSize = regionMag.GetSize();
    typename TOutputImage1::Pointer output = this->GetOutput();


    typename TInputImage2::ConstPointer Phase = this->GetInput2();
    typename TInputImage2::RegionType regionPhase = Phase->GetLargestPossibleRegion();
    // typename TInputImage2::Pointer output = this->GetOutput();
    typename TInputImage2::SizeType regionSize2 = regionPhase.GetSize();

    // Setup output 1
    output->SetRegions(regionPhase);
    output->SetRegions(regionMag);
    output->Allocate();

    /*ImageAlgorithm::Copy(Mag.GetPointer(), output.GetPointer(), output->GetRequestedRegion(),
                         output->GetRequestedRegion() );
    ImageAlgorithm::Copy(Phase.GetPointer(), output.GetPointer(), output->GetRequestedRegion(),
                         output->GetRequestedRegion() );
    */
    // MATLAB formula: S(i).complex = S(i).mag .* exp(-Im.*S(i).phase);
    //  exp(X) is the exponential of the elements of X, e to the X.
    //  For complex Z=X+i*Y, exp(Z) = exp(X)*(COS(Y)+i*SIN(Y)).


    for (int i=0; i<regionSize[0]; i++)
    {

        for (int j=0; j<regionSize[1]; j++)
        {
            for (int m=0; m<regionSize[2]; m++)
            {

                // get phase & mag values
                typename TInputImage1::IndexType Pixel1 ;
                Pixel1[0] = i;
                Pixel1[1] = j;
                Pixel1[2] = m;
                double magVal = Mag->GetPixel(Pixel1);
                double phaseVal = Phase->GetPixel(Pixel1);

                // put into vector pixel
                double X = 0;
                double Y = -phaseVal;

                typename TOutputImage1::PixelType ComplexNum;

                ComplexNum[0] = exp(X) *  cos(Y) * magVal;
                ComplexNum[1] = sin(Y)*magVal;

                output->SetPixel(Pixel1, ComplexNum);
            }
        }
    }


}
template< typename TInputImage1, typename TInputImage2, typename TOutputImage1>
DataObject::Pointer MagToComplex<TInputImage1, TInputImage2, TOutputImage1>::
MakeOutput(unsigned int idx)
{
    DataObject::Pointer output;

    switch ( idx )
    {
    case 0:
        output = ( TOutputImage1::New() ).GetPointer();
        break;
    default:
        std::cerr << "No output " << idx << std::endl;
        output = NULL;
        break;
    }
    return output.GetPointer();
}

template< typename TInputImage1, typename TInputImage2, typename TOutputImage1>
TOutputImage1* MagToComplex<TInputImage1, TInputImage2, TOutputImage1>::GetOutput()
{
    return dynamic_cast< TOutputImage1 * >(this->ProcessObject::GetOutput(0) );
}


}// end namespace


#endif
