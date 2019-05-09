
// --------- March 2017, Ian L Jones ---------
//
//ScaleImage
//    ->SetInput(DOUBLE VOL, mask);
//    ->SetScaleFactor(DOUBLE VAL, MaskScaleFactor);
//    ->GetOutput(DOUBLE VOL);
//


#ifndef __ScaleImage_h
#define __ScaleImage_h

#include "itkImageToImageFilter.h"

namespace itk
{
template< class TImage>
class ScaleImage : public ImageToImageFilter< TImage,  TImage > // This doesn't actually matter, as we will be overriding the output image type in MakeOutput()
{
public:
    /** Standard class typedefs. */
    typedef ScaleImage             Self;
    typedef ImageToImageFilter< TImage,  TImage > Superclass;
    typedef SmartPointer< Self >        Pointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(ScaleImage, ImageToImageFilter);

    // Input images
    void SetInput(const TImage* imIn);

    itkSetGetDecoratedInputMacro(ScaleFactor, double );

    // Output
    TImage* GetOutput();

protected:
    ScaleImage();
    ~ScaleImage() {}

    /** Does the real work. */
    virtual void GenerateData();

    typename TImage::ConstPointer GetInput();

    /**  Create the Output */
    DataObject::Pointer MakeOutput(unsigned int idx);

private:
    ScaleImage(const Self &); //purposely not implemented
    void operator=(const Self &);  //purposely not implemented

};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "ScaleImage.hxx"
#endif


#endif // __ScaleImage_h
