// --------- March 2017, Ian L Jones ---------
//
//GetAngle
//    ->SetInput(DOUBLE VEC VOL, Phi);
//    ->GetOutput(DOUBLE VOL);
//


#ifndef __GetAngle_txx
#define __GetAngle_txx

#include "GetAngle.h"

#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

namespace itk
{

template< typename TInputPhi, typename TOutputWrappedPhase>
GetAngle<TInputPhi, TOutputWrappedPhase>::GetAngle()
{

    this->SetNumberOfRequiredInputs(1);

    this->SetNthOutput( 0, this->MakeOutput(0) );

}

template< typename TInputPhi, typename TOutputWrappedPhase>
void GetAngle<TInputPhi, TOutputWrappedPhase>::SetInput(const TInputPhi* phi)
{
    this->SetNthInput(0, const_cast<TInputPhi*>(phi));
}

template< typename TInputPhi, typename TOutputWrappedPhase >
typename TInputPhi::ConstPointer GetAngle<TInputPhi, TOutputWrappedPhase>::GetInput()
{
    return static_cast< const TInputPhi * >
           ( this->ProcessObject::GetInput(0) );
}

template< typename TInputPhi, typename TOutputWrappedPhase>
void GetAngle<TInputPhi, TOutputWrappedPhase>::GenerateData()
{

    // std::cout << ">>>>>>>>>>>>>>>>>>>this far..."<< std::endl;
    typename TInputPhi::ConstPointer phi = this->GetInput();
    typename TInputPhi::RegionType region = phi->GetLargestPossibleRegion();
    typename TInputPhi::SizeType regionSize = region.GetSize();

    typename TOutputWrappedPhase::Pointer output = this->GetOutput();

    // Setup output 1
    output->SetRegions(regionSize);
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
                typename TInputPhi::IndexType Pixel1 ;
                Pixel1[0] = i;
                Pixel1[1] = j;
                Pixel1[2] = m;
                typename TInputPhi::PixelType phiVal = phi->GetPixel(Pixel1);

                double a = phiVal[0];
                double b = phiVal[1];


                // imITK = imITKVol(160,208,64-sliceNo +1)';
                if(std::abs(a)==0)
                    a = 0;
                if(std::abs(b)==0)
                    b = 0;

                double c = std::atan2(b,a);

               output->SetPixel(Pixel1, c);
            }
        }
    }
}



template< typename TInputPhi, typename TOutputWrappedPhase>
DataObject::Pointer GetAngle<TInputPhi, TOutputWrappedPhase>::
MakeOutput(unsigned int idx)
{
    DataObject::Pointer output;

    switch ( idx )
    {
    case 0:
        output = ( TOutputWrappedPhase::New() ).GetPointer();
        break;
    default:
        std::cerr << "No output " << idx << std::endl;
        output = NULL;
        break;
    }
    return output.GetPointer();
}

template< typename TInputPhi, typename TOutputWrappedPhase>
TOutputWrappedPhase* GetAngle<TInputPhi, TOutputWrappedPhase>::GetOutput()
{
    return dynamic_cast< TOutputWrappedPhase * >(this->ProcessObject::GetOutput(0) );
}


}// end namespace


#endif
