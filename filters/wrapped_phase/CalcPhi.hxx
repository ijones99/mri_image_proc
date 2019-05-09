
// --------- March 2017, Ian L Jones ---------
//
//CalcPhi
//    ->SetInputI0(DOUBLE VEC VOL, I0)
//    ->SetInputI2(DOUBLE VEC VOL, I2);
//    ->GetOutput(DOUBLE VEC VOL)
//
//

#ifndef __CalcPhi_txx
#define __CalcPhi_txx

#include "CalcPhi.h"

#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

namespace itk
{

template< class TImage>
CalcPhi<TImage>::CalcPhi()
{

    this->SetNumberOfRequiredInputs(2);

    this->SetNthOutput( 0, this->MakeOutput(0) );

}

template< class TImage>
void CalcPhi<TImage>::SetInputI0(const TImage* I0)
{
    this->SetNthInput(0, const_cast<TImage*>(I0));
}

template< class TImage>
void CalcPhi<TImage>::SetInputI2(const TImage* I2)
{
    this->SetNthInput(1, const_cast<TImage*>(I2));
}

template< class TImage>
typename TImage::ConstPointer CalcPhi<TImage>::GetInputI0()
{
  return static_cast< const TImage * >
         ( this->ProcessObject::GetInput(0) );
}
template< class TImage>
typename TImage::ConstPointer CalcPhi<TImage>::GetInputI2()
{
  return static_cast< const TImage * >
         ( this->ProcessObject::GetInput(1) );
}


template< class TImage>
void CalcPhi<TImage>::GenerateData()
{

    // std::cout << ">>>>>>>>>>>>>>>>>>>this far..."<< std::endl;
    typename TImage::ConstPointer inputI0 = this->GetInputI0();
    typename TImage::ConstPointer inputI2 = this->GetInputI2();
    typename TImage::Pointer output = this->GetOutput();

    // allocate space for output
    typename TImage::RegionType region = inputI0->GetLargestPossibleRegion();
    typename TImage::SizeType regionSize = region.GetSize();
    output->SetRegions(regionSize);
    output->Allocate();

    // MATLAB formula: phi = (conj(I0).* I2);
    // (x+yi)(u+vi) = (xu-yv) + (xv+yu)i
    // conj(complexX) = complexX - complexXi
    for (int i=0; i<regionSize[0]; i++)
    {

        for (int j=0; j<regionSize[1]; j++)
        {
        for (int m=0; m<regionSize[2]; m++)
        {

            // get phase & mag values
            typename TImage::IndexType Pixel1 ;
            Pixel1[0] = i;
            Pixel1[1] = j;
             Pixel1[2] = m;
            typename TImage::PixelType valI0;
            typename TImage::PixelType valI2;
            valI0 = inputI0->GetPixel(Pixel1);
            valI2 = inputI2->GetPixel(Pixel1);

            typename TImage::PixelType conjVal;
            // calc conj
            conjVal[0] = valI0[0]; conjVal[1] = -valI0[1];
            double x = conjVal[0]; double y = conjVal[1];
            double u = valI2[0]; double v = valI2[1];

            typename TImage::PixelType Phi;
            Phi[0] = (x*u-y*v);
            Phi[1] = (x*v+y*u);
            //std::cout << "Phi: " << Phi[0] << " " << Phi[1] << "i" << std::endl;
            //std::cout << "Phi: " << Phi[0] << " " << Phi[1] << "i" << std::endl;
            output->SetPixel(Pixel1, Phi);
            }
        }
    }


}
template< class TImage> // why is TImage a class when there is only one type and a typename
// when there are multiple image types?
DataObject::Pointer CalcPhi<TImage>::
MakeOutput(unsigned int idx)
{
    DataObject::Pointer output;

    switch ( idx )
    {
    case 0:
        output = ( TImage::New() ).GetPointer();
        break;
    default:
        std::cerr << "No output " << idx << std::endl;
        output = NULL;
        break;
    }
    return output.GetPointer();
}

template< typename TImage>
TImage* CalcPhi<TImage>::GetOutput()
{
    return dynamic_cast< TImage * >(this->ProcessObject::GetOutput(0) );
}


}// end namespace


#endif
