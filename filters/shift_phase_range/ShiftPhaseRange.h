
// --------- March 2017, Ian L Jones ---------
//
//ShiftPhaseRange
//    ->SetInput(DOUBLE VOL, mask);
//    ->SetScaleFactor(DOUBLE VAL, MaskScaleFactor);
//    ->GetOutput(DOUBLE VOL);
//


#ifndef __ShiftPhaseRange_h
#define __ShiftPhaseRange_h

#include "itkImageToImageFilter.h"

namespace itk
{
template< class TImage>
class ShiftPhaseRange : public ImageToImageFilter< TImage,  TImage > // This doesn't actually matter, as we will be overriding the output image type in MakeOutput()
{
public:
    /** Standard class typedefs. */
    typedef ShiftPhaseRange             Self;
    typedef ImageToImageFilter< TImage,  TImage > Superclass;
    typedef SmartPointer< Self >        Pointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(ShiftPhaseRange, ImageToImageFilter);

    // Input images
    void SetInput(const TImage* imIn);

    itkSetGetDecoratedInputMacro(ShiftFactor, double );

    // Output
    TImage* GetOutput();

protected:
    ShiftPhaseRange();
    ~ShiftPhaseRange() {}

    /** Does the real work. */
    virtual void GenerateData();

    typename TImage::ConstPointer GetInput();

    /**  Create the Output */
    DataObject::Pointer MakeOutput(unsigned int idx);

private:
    ShiftPhaseRange(const Self &); //purposely not implemented
    void operator=(const Self &);  //purposely not implemented

};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "ShiftPhaseRange.hxx"
#endif


#endif // __ShiftPhaseRange_h
