// --------- March 2017, Ian L Jones ---------
//
//NormMask
//    ->SetInputMask(DOUBLE VOL, mask);
//    ->GetOutput(DOUBLE VOL);
//

#ifndef __NormMask_txx
#define __NormMask_txx

#include "NormMask.h"

#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

namespace itk
{

template< class TImage>
NormMask<TImage>::NormMask()
{

    this->SetNumberOfRequiredInputs(1);

    this->SetNthOutput( 0, this->MakeOutput(0) );

}

template< class TImage>
void NormMask<TImage>::SetInputMask(const TImage* mask)
{
    this->SetNthInput(0, const_cast<TImage*>(mask));
}

template< class TImage>
typename TImage::ConstPointer NormMask<TImage>::GetInputMask()
{
    return static_cast< const TImage * >
           ( this->ProcessObject::GetInput(0) );
}



template< class TImage>
void NormMask<TImage>::GenerateData()
{


    typename TImage::ConstPointer inputMask = this->GetInputMask();
    typename TImage::Pointer output = this->GetOutput();

    // allocate space for output
    typename TImage::RegionType region = inputMask->GetLargestPossibleRegion();
    typename TImage::SizeType regionSize = region.GetSize();
    output->SetRegions(regionSize);
    output->SetBufferedRegion(output->GetRequestedRegion());
    output->Allocate();

    double maxValMask = 0;
    for (int i=0; i<=regionSize[0]-1; i++)
    {
        for (int j=0; j<=regionSize[1]-1; j++)
        {

            for (int m=0; m<=regionSize[2]-1; m++)
            {
                // get pixel value
                typename TImage::IndexType Pixel1 ;
                Pixel1[0] = i;
                Pixel1[1] = j;
                Pixel1[2] = m;

                // get pixel value

                typename TImage::PixelType valMask = inputMask->GetPixel(Pixel1);

                if(valMask > maxValMask)
                {
                    maxValMask = valMask;
                }
            }
        }
    }



    for (int i=0; i<regionSize[0]; i++)
    {
        for (int j=0; j<regionSize[1]; j++)
        {

            for (int m=0; m<regionSize[2]; m++)
            {
                // get pixel value
                typename TImage::IndexType Pixel1 ;
                Pixel1[0] = i;
                Pixel1[1] = j;
                Pixel1[2] = m;

                // get pixel value

                typename TImage::PixelType valMask = inputMask->GetPixel(Pixel1);

                double aOut = valMask/maxValMask ;

                output->SetPixel(Pixel1, aOut);

            }
        }
    }
}

template< class TImage> // why is TImage a class when there is only one type and a typename
// when there are multiple image types?
DataObject::Pointer NormMask<TImage>::
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
TImage* NormMask<TImage>::GetOutput()
{
    return dynamic_cast< TImage * >(this->ProcessObject::GetOutput(0) );
}


}// end namespace


#endif
