
// --------- March 2017, Ian L Jones ---------
//
//ScaleImage
//    ->SetInput(DOUBLE VOL, mask);
//    ->SetScaleFactor(DOUBLE VAL, MaskScaleFactor);
//    ->GetOutput(DOUBLE VOL);
//

#ifndef __ScaleImage_txx
#define __ScaleImage_txx

#include "ScaleImage.h"

#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"


namespace itk
{

template< class TImage>
ScaleImage<TImage>::ScaleImage()
{

    this->SetNumberOfRequiredInputs(1);

    this->SetNthOutput( 0, this->MakeOutput(0) );

}

template< class TImage>
void ScaleImage<TImage>::SetInput(const TImage* imIn)
{
    this->SetNthInput(0, const_cast<TImage*>(imIn));
}

template< class TImage >
typename TImage::ConstPointer ScaleImage<TImage>::GetInput()
{
    return static_cast< const TImage * >
           ( this->ProcessObject::GetInput(0) );
}

template< class TImage>
void ScaleImage<TImage>::GenerateData()
{


    typename TImage::ConstPointer inputVal = this->GetInput();
    typename TImage::RegionType region = inputVal->GetLargestPossibleRegion();
    typename TImage::SizeType regionSize = region.GetSize();
    typename TImage::Pointer output = this->GetOutput();

    typedef itk::Image<double, 2>   ImageType;

    // Setup output 1
    output->SetRegions(regionSize);
    output->Allocate();

    double scaleFactor = this->GetScaleFactor();
    double maxVolVal = 0;
    double currPixVal = 0;
    typename TImage::IndexType Pixel1 ;


    for (int i=0; i<=regionSize[0]-1; i++)  // x axis
    {
        for (int j=0; j<=regionSize[1]-1; j++) // y axis
        {
            for (int n=0; n<=regionSize[2]-1; n++) // z axis
            {

                Pixel1[0] = i;
                Pixel1[1] = j;
                Pixel1[2] = n;

                currPixVal = inputVal->GetPixel(Pixel1); // get value of volume

                if (currPixVal> maxVolVal)
                {
                    maxVolVal = currPixVal;

                }


            }
        }
    }
    double scaleVal = scaleFactor/maxVolVal;

    for (int i=0; i<regionSize[0]; i++)  // x axis
    {
        for (int j=0; j<regionSize[1]; j++) // y axis
        {
            for (int n=0; n<regionSize[2]; n++) // z axis
            {

                //std::cout <<  "[" << i<< ","<< j<< ","<< k<< ","<< l<< "]" << std::endl;
                Pixel1[0] = i;
                Pixel1[1] = j;
                Pixel1[2] = n;

                currPixVal = inputVal->GetPixel(Pixel1);
/*                if(isnan(currPixVal))
                {
                    currPixVal = 0;
                }

*/
                output->SetPixel(Pixel1, currPixVal*scaleVal);
            }
        }
    }
}



template< class TImage>
DataObject::Pointer ScaleImage<TImage>::
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

template< class TImage>
TImage* ScaleImage<TImage>::GetOutput()
{
    return dynamic_cast< TImage * >(this->ProcessObject::GetOutput(0) );
}


}// end namespace



#endif
