// --------- March 2017, Ian L Jones ---------
//
//Unwrap3D
//    ->SetInputWrappedPhase(DOUBLE VOL, wrappedPhase);
//    ->SetInputMask(UNSIGNED CHAR VOL, maskUChar);
//    ->GetOutput(DOUBLE VOL);
//

#ifndef __Unwrap3D_txx
#define __Unwrap3D_txx

#include "Unwrap3D.h"

#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
//#include "ImageFilterMultipleOutputsDifferentType.h" // add C functions
#include "itkImportImageFilter.h"

namespace itk
{

template< typename TImage, typename TMask, typename TOut>
Unwrap3D<TImage, TMask, TOut>::Unwrap3D()
{

    this->SetNumberOfRequiredInputs(2);

    this->SetNthOutput( 0, this->MakeOutput(0) );

}

template< typename TImage, typename TMask, typename TOut>
void Unwrap3D<TImage, TMask, TOut>::SetInputWrappedPhase(const TImage* wrappedPhase)
{
    this->SetNthInput(0, const_cast<TImage*>(wrappedPhase));
}

template< typename TImage, typename TMask, typename TOut>
void Unwrap3D<TImage, TMask, TOut>::SetInputMask(const TMask* mask)
{
    this->SetNthInput(1, const_cast<TMask*>(mask));
}

template< typename TImage, typename TMask, typename TOut>
typename TImage::ConstPointer Unwrap3D<TImage, TMask, TOut>::GetInputWrappedPhase()
{
    return static_cast< const TImage * >
           ( this->ProcessObject::GetInput(0) );
}
template< typename TImage, typename TMask, typename TOut>
typename TMask::ConstPointer Unwrap3D<TImage, TMask, TOut>::GetInputMask()
{
    return static_cast< const TMask * >
           ( this->ProcessObject::GetInput(1) );
}


template< typename TImage, typename TMask, typename TOut>
void Unwrap3D<TImage, TMask, TOut>::GenerateData()
{

    typedef double   PixelType;
    typedef unsigned char PixelTypeUChar;
    // std::cout << ">>>>>>>>>>>>>>>>>>>this far..."<< std::endl;
    typename TImage::ConstPointer inputWrappedPhase = this->GetInputWrappedPhase();
    typename TMask::ConstPointer inputMask = this->GetInputMask();
    typename TImage::Pointer output = this->GetOutput();

    // allocate space for output
    typename TImage::RegionType region = inputWrappedPhase->GetLargestPossibleRegion();
    typename TImage::SizeType regionSize = region.GetSize();
    //output->SetRegions(regionSize);
    output->SetBufferedRegion(output->GetRequestedRegion());
    output->Allocate();

    // get raw pointer to image
    const unsigned char * maskRaw;
    maskRaw = inputMask->GetBufferPointer();

    const double * wrappedPhaseRaw;
    wrappedPhaseRaw = inputWrappedPhase->GetBufferPointer();

    const int Dimension = 3;
    typedef itk::Image< PixelType, Dimension > ImageType;
    typedef itk::ImportImageFilter< PixelType, Dimension >   ImportFilterType;
    ImportFilterType::SizeType  size;

    size = regionSize;
    int volume_width = size[0];
    int volume_height = size[1];
    int volume_depth = size[2];
    double *UnwrappedVolume;
    int volumeSize = volume_width * volume_height * volume_depth ;
    int numberOfPixels = volume_width * volume_height* volume_depth ;
    unsigned char  *extended_mask;
    int volume_size =volume_width* volume_height* volume_depth;
    int No_of_Edges_initially = 3 * volume_width* volume_height* volume_depth;
    UnwrappedVolume = (double *) calloc(volume_size, sizeof(double));
    VOXELM *voxel = (VOXELM *) calloc(volumeSize, sizeof(VOXELM));
    EDGE *edge = (EDGE *) calloc(No_of_Edges_initially, sizeof(EDGE));;


    extended_mask = (unsigned char *) calloc(numberOfPixels, sizeof(unsigned char));
    PixelTypeUChar * maskRawUCharPtr = const_cast<PixelTypeUChar *>(maskRaw);

    extend_mask(maskRawUCharPtr, extended_mask, volume_width, volume_height, volume_depth);
    PixelType * wrappedPhaseRawPtr = const_cast<PixelType *>(wrappedPhaseRaw);


    initialiseVOXELs(wrappedPhaseRawPtr, maskRawUCharPtr, extended_mask, voxel,
                     volume_width, volume_height, volume_depth);
    calculate_reliability(wrappedPhaseRawPtr, voxel, volume_width, volume_height, volume_depth);
    horizontalEDGEs(voxel, edge, volume_width, volume_height, volume_depth);

    verticalEDGEs(voxel, edge, volume_width, volume_height, volume_depth);

    normalEDGEs(voxel, edge, volume_width, volume_height, volume_depth);

    //sort the EDGEs depending on their reiability. The VOXELs with higher relibility (small value) first
    quicker_sort(edge, edge + No_of_edges - 1);

    //gather VOXELs into groups
    gatherVOXELs(edge);

    unwrapVolume(voxel, volume_width, volume_height, volume_depth);

    // here is where the min value is set
    maskVolume(voxel, maskRawUCharPtr, volume_width, volume_height, volume_depth);

    //copy the volume from VOXELM structure to the unwrapped phase array passed to this function
    returnVolume(voxel, UnwrappedVolume, volume_width, volume_height, volume_depth);

    free(edge);
    free(voxel);


    double * unwrappedOut;
    unwrappedOut = new double[numberOfPixels* sizeof(double)];

    std::copy(UnwrappedVolume, UnwrappedVolume + numberOfPixels, unwrappedOut);
    PixelType * bval = const_cast<PixelType *>(unwrappedOut);

    //PixelType * bval = const_cast<PixelType *>(wrappedPhaseRaw);

    ImportFilterType::Pointer importFilter = ImportFilterType::New();

    ImportFilterType::IndexType start;
    start.Fill( 0 );

    ImportFilterType::RegionType regionImp;
    regionImp.SetIndex( start );
    regionImp.SetSize(  size  );

    importFilter->SetRegion( regionImp );

    const bool importImageFilterWillOwnTheBuffer = true;
    importFilter->SetImportPointer( bval, numberOfPixels,
                                    importImageFilterWillOwnTheBuffer );


    importFilter->Update();


    typename ImageType::Pointer imageImp =  importFilter->GetOutput();

    ImageAlgorithm::Copy(imageImp.GetPointer(), output.GetPointer(), regionImp,
                         output->GetRequestedRegion() );





}
template< typename TImage, typename TMask, typename TOut> // why is TImage a class when there is only one type and a typename
// when there are multiple image types?
DataObject::Pointer Unwrap3D<TImage, TMask, TOut>::
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

template< typename TImage, typename TMask, typename TOut>
TOut* Unwrap3D<TImage, TMask, TOut>::GetOutput()
{
    return dynamic_cast< TOut * >(this->ProcessObject::GetOutput(0) );
}


}// end namespace


#endif
