//
// --------- March 2017, Ian L Jones ---------
//
// FilterTypeDicomLoad
//    ->SetBaseDirName(STRING);
//    ->SetSeriesDir1(STRING); // mag 1 and 2
//    ->SetSeriesDir2(STRING); // phase 1 and 2
//    ->SetSeriesDir3(STRING); // mag 3 and 4
//    ->SetSeriesDir4(STRING); // phase 3 and 4
//    ->GetOutputMag1(DOUBLE VOL);
//    ->GetOutputMag2(DOUBLE VOL);
//    ->GetOutputMag3(DOUBLE VOL);
//    ->GetOutputPhase1(DOUBLE VOL);
//    ->GetOutputPhase2(DOUBLE VOL);
//    ->GetOutputPhase3(DOUBLE VOL);
//
//

#ifndef __LoadDicomFiles_txx
#define __LoadDicomFiles_txx
#include "LoadDicomFiles.h"
#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "math.h"



#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

//#include "ImageFilter.h"

#include "itkMetaDataObject.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include "ShiftPhaseRange.h"


struct IdxCompare;

template <typename TImage>
static void PrintPixelValues(TImage* const image);



double GetMedian(double daArray[], int iSize);
namespace itk
{

template< class TImage>
LoadDicomFiles<TImage>
::LoadDicomFiles()
{
    this->SetNumberOfRequiredInputs(0);
    this->SetNumberOfRequiredOutputs(6);
    this->SetNthOutput( 0, this->MakeOutput(0) );
    this->SetNthOutput( 1, this->MakeOutput(1) );
    this->SetNthOutput( 2, this->MakeOutput(2) );
    this->SetNthOutput( 3, this->MakeOutput(3) );
    this->SetNthOutput( 4, this->MakeOutput(4) );
    this->SetNthOutput( 5, this->MakeOutput(5) );
}

template< class TImage>
void
LoadDicomFiles<TImage>
::GenerateData()
{

    char cwd[256];

    if (getcwd(cwd, sizeof(cwd)) == NULL)
        perror("getcwd() error");
    else
        printf("current working directory is: %s\n", cwd);

    std::string rwDirName= this->GetOutputDirName();
    std::string baseDirName= this->GetBaseDirName();
    // Mag 1 & 2
    std::string seriesDir1Name= this->GetSeriesDir1();
    // Phase 1 & 2
    std::string seriesDir2Name= this->GetSeriesDir2();
    // Mag 3 & 4
    std::string seriesDir3Name= this->GetSeriesDir3();
    // Phase 3 & 4
    std::string seriesDir4Name= this->GetSeriesDir4();
    typedef double PixelType;
    const unsigned int      Dimension = 3;
    typedef itk::Image< PixelType, Dimension >         ImageType;
    typedef itk::ImageSeriesReader< ImageType >        ReaderType;
    typedef itk::MetaDataDictionary   DictionaryType;
    std::string dirName = "."; //current directory by default

    // set up outputs
    typename TImage::Pointer outputMag1 = this->GetOutputMag1();
    typename TImage::Pointer outputMag2 = this->GetOutputMag2();
    typename TImage::Pointer outputMag3 = this->GetOutputMag3();
    typename TImage::Pointer outputPhase1 = this->GetOutputPhase1();
    typename TImage::Pointer outputPhase2 = this->GetOutputPhase2();
    typename TImage::Pointer outputPhase3 = this->GetOutputPhase3();
    typename ImageType::Pointer image;
    std::vector <ReaderType::Pointer> reader(8);
    typedef itk::GDCMImageIO       ImageIOType;
    std::vector <ImageIOType::Pointer> dicomIO(8);

    int magInd []= {0,1,4,5};
    int phaseInd []= {2,3,6,7};

    std::vector<double> teVals(4);
    std::string  seriesDirName;
    int iVolumeCtr = 0;
    int iTECtr = 0;
    for( int iSeries=0; iSeries < 4; iSeries ++ )
    {
        if(iSeries == 0)
        {
            seriesDirName =  seriesDir1Name;
        };
        if(iSeries == 1)
        {
            seriesDirName =  seriesDir2Name;
        };
        if(iSeries == 2)
        {
            seriesDirName =  seriesDir3Name;
        };
        if(iSeries == 3)
        {
            seriesDirName =  seriesDir4Name;
        };

        // seriesDirName is the directory containing all images for a volume
        dirName = baseDirName + "/" + seriesDirName + "/";

        typedef itk::GDCMSeriesFileNames NamesGeneratorType;
        NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
        nameGenerator->SetUseSeriesDetails(true);
        nameGenerator->AddSeriesRestriction("0018|0081"); // filter images by TE
        nameGenerator->SetGlobalWarningDisplay(false);
        nameGenerator->SetInputDirectory(dirName);

        typedef std::vector< std::string >    SeriesIdContainer;
        const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
        SeriesIdContainer::const_iterator seriesItr = seriesUID.begin();
        SeriesIdContainer::const_iterator seriesEnd = seriesUID.end();
        if (seriesItr != seriesEnd)
        {
            std::cout << "The directory: ";
            std::cout << dirName << std::endl;
            std::cout << "Contains the following DICOM Series: ";
            std::cout << std::endl;
        }
        else
        {
            std::cout << "No DICOMs in: " << dirName << std::endl;
        }
        while (seriesItr != seriesEnd)
        {
            std::cout << seriesItr->c_str() << std::endl;
            ++seriesItr;
        }
        seriesItr = seriesUID.begin();
        while (seriesItr != seriesUID.end())
        {
            std::string seriesIdentifier;

            seriesIdentifier = seriesItr->c_str();
            seriesItr++;

            std::cout << "\nReading: ";
            std::cout << seriesIdentifier << std::endl;
            typedef std::vector< std::string >   FileNamesContainer;
            FileNamesContainer fileNames;
            fileNames = nameGenerator->GetFileNames(seriesIdentifier);
            reader[iVolumeCtr] = ReaderType::New();
            dicomIO[iVolumeCtr] = ImageIOType::New();

            reader[iVolumeCtr]->SetImageIO(dicomIO[iVolumeCtr]);
            reader[iVolumeCtr]->SetFileNames(fileNames);
            reader[iVolumeCtr]->Update();


            // START Get DICOM info


            const  DictionaryType & dictionary = dicomIO[iVolumeCtr]->GetMetaDataDictionary();

            typedef itk::MetaDataObject< std::string > MetaDataStringType;

            DictionaryType::ConstIterator itr = dictionary.Begin();
            DictionaryType::ConstIterator endItr = dictionary.End();

            std::string entryId = "0018|0081";
            DictionaryType::ConstIterator tagItr = dictionary.Find( entryId );
            if( tagItr == endItr )
            {
                std::cerr << "Tag " << entryId;
                std::cerr << " not found in the DICOM header" << std::endl;
            }

            MetaDataStringType::ConstPointer entryvalue =
                dynamic_cast<const MetaDataStringType *>( tagItr->second.GetPointer() );
            if( entryvalue && (iVolumeCtr == 2 || iVolumeCtr == 3 || iVolumeCtr == 6 || iVolumeCtr == 7))
            {
                std::string orbits = entryvalue->GetMetaDataObjectValue();
                teVals[iTECtr] = std::atof(orbits.c_str());
                ++iTECtr;

            }


            // END Get DICOM info


            ++iVolumeCtr;
            std::cout << "iVol: " << iVolumeCtr << std::endl;
        }



    }
    // sort teVals
    std::vector<int> y;

    // initialize indexes
    for(int ii = 0; ii < 4; ++ii)
        y.push_back(ii);

    std::sort(y.begin(), y.end(), IdxCompare(teVals));
    for(int ii = 0; ii < 4; ++ii)
        std::cout << y[ii] << std::endl;
    // assign output Mag and Phase (delete 2nd loaded)


    typedef itk::ShiftPhaseRange< TImage> ShiftFilterType;
    typename ShiftFilterType::Pointer shiftFilter1 = ShiftFilterType::New();
    shiftFilter1->SetInput(reader[ phaseInd[y[0]]]->GetOutput());
    shiftFilter1->SetShiftFactor(4096);
    typename TImage::Pointer phase1Shifted = shiftFilter1->GetOutput();

    typedef itk::ShiftPhaseRange< TImage> ShiftFilterType;
    typename ShiftFilterType::Pointer  shiftFilter2 = ShiftFilterType::New();
    shiftFilter2->SetInput(reader[ phaseInd[y[2]]]->GetOutput());
    shiftFilter2->SetShiftFactor(4096);
    typename TImage::Pointer phase2Shifted = shiftFilter2->GetOutput();

    typedef itk::ShiftPhaseRange< TImage> ShiftFilterType;
    typename ShiftFilterType::Pointer  shiftFilter3 = ShiftFilterType::New();
    shiftFilter3->SetInput(reader[ phaseInd[y[3]]]->GetOutput());
    shiftFilter3->SetShiftFactor(4096);
    typename TImage::Pointer phase3Shifted = shiftFilter3->GetOutput();

    // Save files for checking (debugging)
    // MAGNITUDE
    typedef  itk::ImageFileWriter< TImage  > WriterType;
    typename WriterType::Pointer writer = WriterType::New();
    // save original file
    writer->SetFileName(rwDirName + "mag_loaded_275.mha");
    writer->SetInput(reader[ magInd[y[0]]]->GetOutput());
    writer->Update();

    writer = WriterType::New();
    // save original file
    writer->SetFileName(rwDirName + "mag_loaded_395.mha");
    writer->SetInput(reader[ magInd[y[2]]]->GetOutput());
    writer->Update();

    writer = WriterType::New();
    // save original file
    writer->SetFileName(rwDirName + "mag_loaded_515.mha");
    writer->SetInput(reader[ magInd[y[3]]]->GetOutput());
    writer->Update();

    // PHASE
    writer = WriterType::New();
    // save original file
    writer->SetFileName(rwDirName + "phase_loaded_275.mha");
    writer->SetInput(phase1Shifted);
    writer->Update();

    writer = WriterType::New();
    // save original file
    writer->SetFileName(rwDirName + "phase_loaded_395.mha");
    writer->SetInput(phase2Shifted);
    writer->Update();

    writer = WriterType::New();
    // save original file
    writer->SetFileName(rwDirName + "phase_loaded_515.mha");
    writer->SetInput( phase3Shifted);
    writer->Update();


    // Magnitudes

    this->AllocateOutputs();
    typename TImage::Pointer image1 = reader[ magInd[y[0]]]->GetOutput();
    typename TImage::RegionType region1 = image1->GetLargestPossibleRegion();
    outputMag1->SetRegions(region1);
    outputMag1->Allocate();
    ImageAlgorithm::Copy(image1.GetPointer(), outputMag1.GetPointer(), outputMag1->GetLargestPossibleRegion(),
                         outputMag1->GetLargestPossibleRegion() );

    typename TImage::Pointer image2 = reader[ magInd[y[2]]]->GetOutput();
    typename TImage::RegionType region2 = image2->GetLargestPossibleRegion();
    outputMag2->SetRegions(region2);
    outputMag2->Allocate();
    ImageAlgorithm::Copy(image2.GetPointer(), outputMag2.GetPointer(), outputMag2->GetLargestPossibleRegion(),
                         outputMag2->GetLargestPossibleRegion() );

    typename TImage::Pointer image3 = reader[ magInd[y[3]]]->GetOutput();
    typename TImage::RegionType region3 = image3->GetLargestPossibleRegion();
    outputMag3->SetRegions(region3);
    outputMag3->Allocate();
    ImageAlgorithm::Copy(image3.GetPointer(), outputMag3.GetPointer(), outputMag3->GetLargestPossibleRegion(),
                         outputMag3->GetLargestPossibleRegion() );

    // Phases
    typename TImage::Pointer image4 = shiftFilter1->GetOutput();
    typename TImage::RegionType region4 = phase1Shifted->GetLargestPossibleRegion();
    outputPhase1->SetRegions(region4);
    outputPhase1->Allocate();
    ImageAlgorithm::Copy(phase1Shifted.GetPointer(), outputPhase1.GetPointer(), outputPhase1->GetLargestPossibleRegion(),
                         outputPhase1->GetLargestPossibleRegion() );

    typename TImage::Pointer image5 = shiftFilter2->GetOutput();
    typename TImage::RegionType region5 = phase2Shifted->GetLargestPossibleRegion();
    outputPhase2->SetRegions(region5);
    outputPhase2->Allocate();
    ImageAlgorithm::Copy(phase2Shifted.GetPointer(), outputPhase2.GetPointer(), outputPhase2->GetLargestPossibleRegion(),
                         outputPhase2->GetLargestPossibleRegion() );

    typename TImage::Pointer image6 = shiftFilter3->GetOutput();
    typename TImage::RegionType region6 = phase3Shifted->GetLargestPossibleRegion();
    outputPhase3->SetRegions(region6);
    outputPhase3->Allocate();
    ImageAlgorithm::Copy(phase3Shifted.GetPointer(), outputPhase3.GetPointer(), outputPhase3->GetLargestPossibleRegion(),
                         outputPhase3->GetLargestPossibleRegion() );





}


template< class TImage>
DataObject::Pointer LoadDicomFiles<TImage>::MakeOutput(unsigned int idx)
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
    case 2:
        output = ( TImage::New() ).GetPointer();
        break;
    case 3:
        output = ( TImage::New() ).GetPointer();
        break;
    case 4:
        output = ( TImage::New() ).GetPointer();
        break;
    case 5:
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
TImage* LoadDicomFiles<TImage>::GetOutputMag1()
{
    return dynamic_cast< TImage * >(
               this->ProcessObject::GetOutput(0) );
}

template< class TImage>
TImage* LoadDicomFiles<TImage>::GetOutputMag2()
{
    return dynamic_cast< TImage * >(
               this->ProcessObject::GetOutput(1) );
}

template< class TImage>
TImage* LoadDicomFiles<TImage>::GetOutputMag3()
{
    return dynamic_cast< TImage * >(
               this->ProcessObject::GetOutput(2) );
}

template< class TImage>
TImage* LoadDicomFiles<TImage>::GetOutputPhase1()
{
    return dynamic_cast< TImage * >(
               this->ProcessObject::GetOutput(3) );
}

template< class TImage>
TImage* LoadDicomFiles<TImage>::GetOutputPhase2()
{
    return dynamic_cast< TImage * >(
               this->ProcessObject::GetOutput(4) );
}
template< class TImage>
TImage* LoadDicomFiles<TImage>::GetOutputPhase3()
{
    return dynamic_cast< TImage * >(
               this->ProcessObject::GetOutput(5) );
}

}// end namespace

struct IdxCompare
{
    const std::vector<double>& target;

    IdxCompare(const std::vector<double>& target): target(target) {}

    bool operator()(int a, int b) const
    {
        return target[a] < target[b];
    }
};

template <typename TImage>
void PrintPixelValues(TImage* const image)
{
    typename TImage::IndexType pixelIndex;
    for (int i=102-1; i<102; i++) // x direction
    {
        typename TImage::RegionType region = image->GetLargestPossibleRegion();
        typename TImage::SizeType regionSize = region.GetSize();
        for (int j=74-1; j<92; j++) // y direction
        {
            pixelIndex[0] = i;
            pixelIndex[1] = j;
            std::cout << image->GetPixel(pixelIndex) << std::endl;
        }


    }

}



#endif
