/*=========================================================================
 *
 *  Ian L Jones 30.01.2017
 *  IN: vector (complex number representation)
 *  OUT: scalar
 *
 *  MATLAB code to translate
 *      mask = 255*ones(size(S(1).complex));
 *      mask(abs(S(1).complex) < mask_threshold) = NaN;
 *      mask = mask/255;
 *      wrapped_phase = wrapped_phase.*mask;
 *      wrapped_phase_degree = wrapped_phase*180/pi;
 *      wrapped_phase_degree = wrapped_phase_degree + 360;
 *
 *=========================================================================*/

#ifndef __CreateWrappedPhaseDegree_txx
#define __CreateWrappedPhaseDegree_txx
#include "CreateWrappedPhaseDegree.h"
#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

template <typename TImage>
static void CreateImage(TImage* const image);

namespace itk
{

template< typename TInput, typename TOutput>
CreateWrappedPhaseDegree<TInput, TOutput>::CreateWrappedPhaseDegree()
{

    this->SetNumberOfRequiredInputs(1);

    this->SetNthOutput( 0, this->MakeOutput(0) );

}

template< typename TInput, typename TOutput>
void CreateWrappedPhaseDegree<TInput, TOutput>::SetInput(const TInput* phi)
{
    this->SetNthInput(0, const_cast<TInput*>(phi));
}

template< typename TInput, typename TOutput >
typename TInput::ConstPointer CreateWrappedPhaseDegree<TInput, TOutput>::GetInput()
{
    return static_cast< const TInput * >
           ( this->ProcessObject::GetInput(0) );
}

template< typename TInput, typename TOutput>
void CreateWrappedPhaseDegree<TInput, TOutput>::GenerateData()
{
    // define pixel types
    typedef itk::Vector< float, 2 >       PixelTypeV;
    typedef itk::Image< PixelTypeV, 2 >    ImageTypeV;
    typedef itk::Image<float, 2>   PixelTypeSca;
    typedef itk::Image< PixelTypeSca, 2 >    ImageTypeSca;

    // std::cout << ">>>>>>>>>>>>>>>>>>>this far..."<< std::endl;
    typename TInput::ConstPointer wrapped_phase = this->GetInput();
    typename TInput::RegionType region = wrapped_phase->GetLargestPossibleRegion();
    typename TInput::SizeType regionSize = region.GetSize();


    // create init mask
    ImageTypeSca::Pointer mask = ImageTypeSca::New();
    CreateImage(mask.GetPointer());
    mask->SetRegions(regionSize);

    // Setup output 1

    typename TOutput::Pointer output = this->GetOutput();
    output->SetRegions(regionSize);
    output->Allocate();

    for (int i=0; i<regionSize[0]; i++)

        for (int j=0; j<regionSize[1]; j++)
        {

        /*
            // get phase & mag values
            typename TInput::IndexType Pixel1 ;
            Pixel1[0] = i;
            Pixel1[1] = j;
            typename ImageType::PixelType phiVal = phi->GetPixel(Pixel1);

            float a = phiVal[0];
            float b = phiVal[1];
            float c = atan2(b,a);
*/
            /*if (i==102-1 && j==74-1)
            {
                std::cout <<   "first vals: X=" << X << " " << "Y=" << Y << ": " <<ComplexNum[0] <<" " << ComplexNum[1] << "i"<<std::endl;
            }
            */
            //output->SetPixel(Pixel1, c);
        }
}



template< typename TInput, typename TOutput>
DataObject::Pointer CreateWrappedPhaseDegree<TInput, TOutput>::
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

template< typename TInput, typename TOutput>
TOutput* CreateWrappedPhaseDegree<TInput, TOutput>::GetOutput()
{
    return dynamic_cast< TOutput * >(this->ProcessObject::GetOutput(0) );
}


}// end namespace

template <typename TImage>
void CreateImage(TImage* const image)
{
    // Create an image with 2 connected components
    typename TImage::IndexType corner = {{0,0}};

    unsigned int NumRows = 200;
    unsigned int NumCols = 300;
    typename TImage::SizeType size = {{NumRows, NumCols}};

    typename TImage::RegionType region(corner, size);

    image->SetRegions(region);
    image->Allocate();

    image->FillBuffer(0);
}


#endif
