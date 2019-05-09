
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

#ifndef __LoadDicomFiles_h
#define __LoadDicomFiles_h

#include "itkImageToImageFilter.h"

namespace itk
{
template<class TImage>
class LoadDicomFiles : public ImageToImageFilter< TImage, TImage  > // This doesn't actually matter, as we will be overriding the output image type in MakeOutput()
{
public:
    /** Standard class typedefs. */
    typedef LoadDicomFiles             Self;
    typedef ImageToImageFilter< TImage, TImage  > Superclass;
    typedef SmartPointer< Self >        Pointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(LoadDicomFiles, ImageToImageFilter);

    // Input images
    itkSetGetDecoratedInputMacro(BaseDirName, std::string  );
    itkSetGetDecoratedInputMacro(OutputDirName, std::string  );
    itkSetGetDecoratedInputMacro(SeriesDir1, std::string  );
    itkSetGetDecoratedInputMacro(SeriesDir2, std::string  );
    itkSetGetDecoratedInputMacro(SeriesDir3, std::string  );
    itkSetGetDecoratedInputMacro(SeriesDir4, std::string  );

    // Output
    TImage* GetOutputMag1();
    TImage* GetOutputMag2();
    TImage* GetOutputMag3();

    TImage* GetOutputPhase1();
    TImage* GetOutputPhase2();
    TImage* GetOutputPhase3();

protected:
    LoadDicomFiles();
    ~LoadDicomFiles() {}

    /** Does the real work. */
    virtual void GenerateData();

    /**  Create the Output */
    DataObject::Pointer MakeOutput(unsigned int  idx);

private:
    LoadDicomFiles(const Self &); //purposely not implemented
    void operator=(const Self &);  //purposely not implemented

};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "LoadDicomFiles.hxx"
#endif


#endif // __LoadDicomFiles_h
