// --------- March 2017, Ian L Jones ---------
//
//Unwrap3D
//    ->SetInputWrappedPhase(DOUBLE VOL, wrappedPhase);
//    ->SetInputMask(UNSIGNED CHAR VOL, maskUChar);
//    ->GetOutput(DOUBLE VOL);
//

#ifndef __Unwrap3D_h
#define __Unwrap3D_h

#include "itkImageToImageFilter.h"

namespace itk
{
template< typename TImage, typename TMask, typename TOut>
class Unwrap3D : public ImageToImageFilter< TImage, TOut> // This doesn't actually matter, as we will be overriding the output image type in MakeOutput()
{
public:
    /** Standard class typedefs. */
    typedef Unwrap3D             Self;
    typedef ImageToImageFilter< TImage,  TOut> Superclass;
    typedef SmartPointer< Self >        Pointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(Unwrap3D, ImageToImageFilter);

    // Input images
    void SetInputWrappedPhase(const TImage* wrappedPhase);
    void SetInputMask(const TMask* mask);

    // Output
    TOut* GetOutput();

protected:
    Unwrap3D();
    ~Unwrap3D() {}

    /** Does the real work. */
    virtual void GenerateData();

    typename TImage::ConstPointer GetInputWrappedPhase();
    typename TMask::ConstPointer GetInputMask();

    // Create the Output
    DataObject::Pointer MakeOutput(unsigned int idx);

private:
    Unwrap3D(const Self &); //purposely not implemented
    void operator=(const Self &);  //purposely not implemented

};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATI0N
#include "Unwrap3D.hxx"
#endif


#endif // __Unwrap3D_h
