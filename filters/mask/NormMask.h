// --------- March 2017, Ian L Jones ---------
//
//NormMask
//    ->SetInputMask(DOUBLE VOL, mask);
//    ->GetOutput(DOUBLE VOL);
//




#ifndef __NormMask_h
#define __NormMask_h

#include "itkImageToImageFilter.h"

namespace itk
{
template< class TImage>
class NormMask : public ImageToImageFilter< TImage, TImage> // This doesn't actually matter, as we will be overriding the output image type in MakeOutput()
{
public:
    /** Standard class typedefs. */
    typedef NormMask             Self;
    typedef ImageToImageFilter< TImage,  TImage > Superclass;
    typedef SmartPointer< Self >        Pointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(NormMask, ImageToImageFilter);

    // Input images
    void SetInputMask(const TImage* mask);


    // Output
    TImage* GetOutput();

protected:
    NormMask();
    ~NormMask() {}

    /** Does the real work. */
    virtual void GenerateData();

    typename TImage::ConstPointer GetInputMask();


    // Create the Output
    DataObject::Pointer MakeOutput(unsigned int idx);

private:
    NormMask(const Self &); //purposely not implemented
    void operator=(const Self &);  //purposely not implemented

};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATI0N
#include "NormMask.hxx"
#endif


#endif // __NormMask_h
