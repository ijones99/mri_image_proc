// --------- March 2017, Ian L Jones ---------
//
//FilterFatContent
//    ->SetInputFr(DOUBLE VOL, fr);
//    ->SetInputWc(DOUBLE VOL, wcMask);
//    ->SetInputFc(DOUBLE VOL, fcMask);
//    ->GetOutputFc(DOUBLE VOL );
//    ->GetOutputWc(DOUBLE VOL );
//
//
//


#ifndef __FilterFatContent_txx
#define __FilterFatContent_txx
#include "FilterFatContent.h"
#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "math.h"

double GetMedian(double daArray[], int iSize);

namespace itk
{

template< class TImage>
FilterFatContent<TImage>
::FilterFatContent()
{
    this->SetNumberOfRequiredInputs(3);
    this->SetNumberOfRequiredOutputs(2);
    this->SetNthOutput( 0, this->MakeOutput(0) );
    this->SetNthOutput( 1, this->MakeOutput(1) );
}

template< class TImage>
void FilterFatContent<TImage>
::SetInputFr(const TImage* fr)
{
    this->SetNthInput(0, const_cast<TImage*>(fr));
}

template< class TImage>
void FilterFatContent<TImage>
::SetInputWc(const TImage* wc)
{
    this->SetNthInput(1, const_cast<TImage*>(wc));
}

template< class TImage>
void FilterFatContent<TImage>
::SetInputFc(const TImage* fc)
{
    this->SetNthInput(2, const_cast<TImage*>(fc));
}
//------------------------------------------------
template< class TImage>
typename TImage::ConstPointer FilterFatContent<TImage>::GetInputFr()
{
    return static_cast< const TImage * >
           ( this->ProcessObject::GetInput(0) );
}
template< class TImage>
typename TImage::ConstPointer FilterFatContent<TImage>::GetInputWc()
{
    return static_cast< const TImage * >
           ( this->ProcessObject::GetInput(1) );
}
template< class TImage>
typename TImage::ConstPointer FilterFatContent<TImage>::GetInputFc()
{
    return static_cast< const TImage * >
           ( this->ProcessObject::GetInput(2) );
}

template< class TImage>
void
FilterFatContent<TImage>
::GenerateData()
{

    typename TImage::ConstPointer aPtr = this->GetInputFr();
    typename TImage::ConstPointer bPtr = this->GetInputWc();
    typename TImage::ConstPointer cPtr = this->GetInputFc();

    typename TImage::RegionType region = aPtr->GetLargestPossibleRegion();
    typename TImage::SizeType regionSize = region.GetSize();

    typename TImage::Pointer outputFc = this->GetOutputFc();
    outputFc->SetRegions(regionSize);
    outputFc->Allocate();


    typename TImage::Pointer outputWc = this->GetOutputWc();


    outputWc->SetRegions(regionSize);
    outputWc->Allocate();

    double NANval = std::numeric_limits<unsigned char >::quiet_NaN();

    for (int i=0; i<regionSize[0]-1; i++)
    {
        for (int j=0; j<regionSize[1]-1; j++)
        {
            for (int m=0; m<regionSize[2]; m++)
            {
                // get pixel value
                typename TImage::IndexType Pixel1 ;
                Pixel1[0] = i;
                Pixel1[1] = j;
                Pixel1[2] = m;
                typename TImage::IndexType PixelInit = Pixel1 ;
                // get pixel value
                typename TImage::PixelType A = aPtr->GetPixel(Pixel1);
                typename TImage::PixelType B = bPtr->GetPixel(Pixel1);
                typename TImage::PixelType C = cPtr->GetPixel(Pixel1);
                int x = i;
                int y = j;
                double bRegion[] = {0, 0, 0, 0};
                double cRegion[] = {0, 0, 0, 0};


                /*
                             [x,y] = find(A == 50);

                        for j = 1:size(x,1)

                            if x(j) ~= 1 && y(j) ~=1 && x(j) ~= size(A,1) && y(j) ~= size(A,2)
                                B(x(j),y(j)) = median([B(x(j)-1,y(j)) B(x(j)+1,y(j)) B(x(j),y(j)-1) B(x(j),y(j)+1)]);
                                C(x(j),y(j)) = median([C(x(j)-1,y(j)) C(x(j)+1,y(j)) C(x(j),y(j)-1) C(x(j),y(j)+1)]);

                                wc(x(j),y(j),i) = B(x(j),y(j));
                                fc(x(j),y(j),i) = C(x(j),y(j));

                            else
                                wc(x(j),y(j),i) = NaN;
                                fc(x(j),y(j),i) = NaN;
                            end
                        end */

                if (A == 50)
                {
                    if(x!=0 && y!=0 && x != regionSize[0]-1 && y != regionSize[1]-1)
                    {

                        // fill arrays
                        Pixel1[0] = i-1;
                        Pixel1[1] = j;
                        bRegion[0] = bPtr->GetPixel(Pixel1);
                        cRegion[0] = cPtr->GetPixel(Pixel1);
                        Pixel1[0] = i+1;
                        Pixel1[1] = j;
                        bRegion[1] = bPtr->GetPixel(Pixel1);
                        cRegion[1] = cPtr->GetPixel(Pixel1);
                        Pixel1[0] = i;
                        Pixel1[1] = j-1;
                        bRegion[2] = bPtr->GetPixel(Pixel1);
                        cRegion[2] = cPtr->GetPixel(Pixel1);
                        Pixel1[0] = i;
                        Pixel1[1] = j+1;
                        bRegion[3] = bPtr->GetPixel(Pixel1);
                        cRegion[3] = cPtr->GetPixel(Pixel1);

                        B = GetMedian(bRegion, 4);
                        C = GetMedian(cRegion, 4);

                        if(i== 72-1&& j == 351-1&& m == 21-1)
                        {

                            std::cout << "region vals = " << cRegion[0] << ", " << cRegion[1] << ", "<< cRegion[2] << ", "<< cRegion[3] << ", "  << std::endl;
                            std::cout << "C = " << C<< std::endl;
                        }
                        outputWc->SetPixel(PixelInit, B);
                        outputFc->SetPixel(PixelInit, C);

                    }
                    else
                    {
                        outputWc->SetPixel(PixelInit, NANval);
                        outputFc->SetPixel(PixelInit, NANval);
                    }
                }
                else
                {

                    bRegion[3] = bPtr->GetPixel(Pixel1);
                    cRegion[3] = cPtr->GetPixel(Pixel1);

                    outputWc->SetPixel(PixelInit, bPtr->GetPixel(PixelInit));
                    outputFc->SetPixel(PixelInit, cPtr->GetPixel(PixelInit));


                }

            }
        }
    }

}


template< class TImage>
DataObject::Pointer FilterFatContent<TImage>::MakeOutput(unsigned int idx)
{
    DataObject::Pointer output;

    switch ( idx )
    {
    case 0:
        output = ( TImage::New() ).GetPointer();
        break;
    case 1:
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
TImage* FilterFatContent<TImage>::GetOutputFc()
{
    return dynamic_cast< TImage * >(
               this->ProcessObject::GetOutput(0) );
}

template< class TImage>
TImage* FilterFatContent<TImage>::GetOutputWc()
{
    return dynamic_cast< TImage * >(
               this->ProcessObject::GetOutput(1) );
}

}// end namespace

double GetMedian(double daArray[], int iSize)
{
    // Allocate an array of the same size and sort it.
    double* dpSorted = new double[iSize];


    for (int i = 0; i < iSize; ++i)
    {
        dpSorted[i] = daArray[i];

    }
    double dMedian;


        for (int i = iSize - 1; i > 0; --i)
        {
            for (int j = 0; j < i; ++j)
            {
                if (dpSorted[j] > dpSorted[j+1])
                {
                    double dTemp = dpSorted[j];
                    dpSorted[j] = dpSorted[j+1];
                    dpSorted[j+1] = dTemp;
                }
            }
        }

        // Middle or average of middle values in the sorted array.
        dMedian = 0.0;
        if ((iSize % 2) == 0)
        {
            dMedian = (dpSorted[iSize/2] + dpSorted[(iSize/2) - 1])/2.0;
        }
        else
        {
            dMedian = dpSorted[iSize/2];
        }
        delete [] dpSorted;

    return dMedian;
}

#endif
