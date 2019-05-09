
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


#ifndef __FilterFatContent_h
#define __FilterFatContent_h

#include "itkImageToImageFilter.h"

namespace itk
{
template<class TImage>
class FilterFatContent : public ImageToImageFilter< TImage, TImage  > // This doesn't actually matter, as we will be overriding the output image type in MakeOutput()
{
public:
    /** Standard class typedefs. */
    typedef FilterFatContent             Self;
    typedef ImageToImageFilter< TImage, TImage  > Superclass;
    typedef SmartPointer< Self >        Pointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(FilterFatContent, ImageToImageFilter);

    // Input images
    void SetInputFr(const TImage* fr);
    void SetInputWc(const TImage* wc);
    void SetInputFc(const TImage* fc);

    // Output
    TImage* GetOutputFc();
    TImage* GetOutputWc();

protected:
    FilterFatContent();
    ~FilterFatContent() {}

    /** Does the real work. */
    virtual void GenerateData();

    typename TImage::ConstPointer GetInputFr();
    typename TImage::ConstPointer GetInputWc();
    typename TImage::ConstPointer GetInputFc();

    /**  Create the Output */
    DataObject::Pointer MakeOutput(unsigned int  idx);

private:
    FilterFatContent(const Self &); //purposely not implemented
    void operator=(const Self &);  //purposely not implemented

};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "FilterFatContent.hxx"
#endif


#endif // __FilterFatContent_h
