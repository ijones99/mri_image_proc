
// --------- March 2017, Ian L Jones ---------
//
//Transpose
//    ->SetInput(DOUBLE VOL, mask);
//    ->SetScaleFactor(DOUBLE VAL, MaskScaleFactor);
//    ->GetOutput(DOUBLE VOL);
//


#ifndef __Transpose_h
#define __Transpose_h

#include "itkImageToImageFilter.h"

namespace itk
{
template< class TImage>
class Transpose : public ImageToImageFilter< TImage,  TImage > // This doesn't actually matter, as we will be overriding the output image type in MakeOutput()
{
public:
    /** Standard class typedefs. */
    typedef Transpose             Self;
    typedef ImageToImageFilter< TImage,  TImage > Superclass;
    typedef SmartPointer< Self >        Pointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(Transpose, ImageToImageFilter);

    // Input images
    void SetInput(const TImage* imIn);

    // Output
    TImage* GetOutput();

protected:
    Transpose();
    ~Transpose() {}

    /** Does the real work. */
    virtual void GenerateData();

    typename TImage::ConstPointer GetInput();

    /**  Create the Output */
    DataObject::Pointer MakeOutput(unsigned int idx);

private:
    Transpose(const Self &); //purposely not implemented
    void operator=(const Self &);  //purposely not implemented

};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "Transpose.hxx"
#endif


#endif // __Transpose_h
