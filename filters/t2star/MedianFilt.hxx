// --------- March 2017, Ian L Jones ---------
//
//MedianFilt
//    ->SetInput(DOUBLE VOL, t2StarUnfilt);
//    ->SetFilterSizeX(DOUBLE VAL, CELL_FILTER[0]);
//    ->SetFilterSizeY(DOUBLE VAL, CELL_FILTER[1]);
//    ->GetOutput(DOUBLE VOL);
//

#ifndef __MedianFilt_txx
#define __MedianFilt_txx

#include "MedianFilt.h"

#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

float GetMed(float daArray[], int iSize);

namespace itk
{

template< typename TInput, typename TOutput>
MedianFilt<TInput, TOutput>::MedianFilt()
{

    this->SetNumberOfRequiredInputs(1);

    this->SetNthOutput( 0, this->MakeOutput(0) );

}

template< typename TInput, typename TOutput>
void MedianFilt<TInput, TOutput>::SetInput(const TInput* imIn)
{
    this->SetNthInput(0, const_cast<TInput*>(imIn));
}

template< typename TInput, typename TOutput >
typename TInput::ConstPointer MedianFilt<TInput, TOutput>::GetInput()
{
    return static_cast< const TInput * >
           ( this->ProcessObject::GetInput(0) );
}

template< typename TInput, typename TOutput>
void MedianFilt<TInput, TOutput>::GenerateData()
{

    // std::cout << ">>>>>>>>>>>>>>>>>>>this far..."<< std::endl;
    typename TInput::ConstPointer inputVal = this->GetInput();
    typename TInput::RegionType region = inputVal->GetLargestPossibleRegion();
    typename TInput::SizeType regionSize = region.GetSize();

    typename TOutput::Pointer output = this->GetOutput();

    typedef itk::Image<float, 2>   ImageType;


    // Setup output 1
    output->SetRegions(regionSize);
    output->Allocate();

    /*
    if cell_filter > 0
    for i = 1:size(T2star,3)
        t2star = squeeze(T2star(:,:,i));
        T2star(:,:,i) = medfilt2(t2star,[cell_filter cell_filter]);
    end
    end

    */
    int CELL_FILTER[2];
    CELL_FILTER[0] = this->GetFilterSizeX();
    CELL_FILTER[1] = this->GetFilterSizeY();
    //const int CELL_FILTER[]  = {2,2};
    int intRoiSize = CELL_FILTER[0]*CELL_FILTER[1];
    float roiArea[intRoiSize ];


    typename TInput::IndexType Pixel1 ;
    typename TOutput::IndexType Pixel2 ;
    typename TInput::PixelType currPixVal;
    float medFilt ;
    int p = 0;



    // reverse order of iteration due to transpose between data in Matlab and ITK
    for (int n=0; n<regionSize[2]; n++) // z axis
    {

        for (int i=0; i<regionSize[0]-CELL_FILTER[0]-1; i++)  // x axis
        {
            for (int j=0; j<regionSize[1]-CELL_FILTER[1]-1; j++) // y axis
            {

                int m = 0;
                for (int k=i; k < i+CELL_FILTER[0]; k++) // x in ROI
                {
                    for(int l=j; l<j+CELL_FILTER[1]; l++) // y in ROI
                    {

                        //std::cout <<  "[" << i<< ","<< j<< ","<< k<< ","<< l<< "]" << std::endl;
                        Pixel1[0] = k;
                        Pixel1[1] = l;
                        Pixel1[2] = n;

                        currPixVal = inputVal->GetPixel(Pixel1);
                        roiArea[m] = currPixVal;
                        m++;
                    }

                }
                Pixel2[0] = i;
                Pixel2[1] = j;
                Pixel2[2] = n;

                medFilt = 0;

                medFilt = GetMed(roiArea, intRoiSize);

               /* if(p > 109700 && p < 109725 )
                {
                    std::cout <<  "med vector " << roiArea[0] << ","  << roiArea[1] << "," << roiArea[2]<< "," << roiArea[3] << std::endl;
                    std::cout <<  "median = " << medFilt << std::endl;
                }*/
                p++;
                output->SetPixel(Pixel2, medFilt);
            }
        }
    }
}



template< typename TInput, typename TOutput>
DataObject::Pointer MedianFilt<TInput, TOutput>::
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
TOutput* MedianFilt<TInput, TOutput>::GetOutput()
{
    return dynamic_cast< TOutput * >(this->ProcessObject::GetOutput(0) );
}


}// end namespace

float GetMed(float daArray[], int iSize)
{
    // Allocate an array of the same size and sort it.
    double* dpSorted = new double[iSize];
    for (int i = 0; i < iSize; ++i)
    {
        dpSorted[i] = daArray[i];
    }
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
    double dMedian = 0.0;
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
