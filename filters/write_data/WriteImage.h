/*=========================================================================
 *
 *  Ian L Jones 30.01.2017
 *  IN: vector (complex number representation), (complex number representation)
 *  OUT: vector (complex number representation)
 *
 *=========================================================================*/




#ifndef __WriteImage_h
#define __WriteImage_h

#include "itkImageToImageFilter.h"

namespace itk
{
template< class TImage>
class WriteImage : public ImageToImageFilter< TImage, TImage> // This doesn't actually matter, as we will be overriding the output image type in MakeOutput()
{
public:
    /** Standard class typedefs. */
    typedef WriteImage             Self;
    typedef ImageToImageFilter< TImage,  TImage > Superclass;
    typedef SmartPointer< Self >        Pointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(WriteImage, ImageToImageFilter);

    // Input images
    void SetInput(const TImage* image);

    itkSetGetDecoratedInputMacro(FileName, std::string);

protected:
    WriteImage();
    ~WriteImage() {}

    /** Does the real work. */
    virtual void GenerateData();

    typename TImage::ConstPointer GetInput();

    // Create the Output
    DataObject::Pointer MakeOutput(unsigned int idx);

private:
    WriteImage(const Self &); //purposely not implemented
    void operator=(const Self &);  //purposely not implemented

};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATI0N
#include "WriteImage.hxx"
#endif


#endif // __WriteImage_h
