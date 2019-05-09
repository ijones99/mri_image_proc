// --------- March 2017, Ian L Jones ---------
//
//ApplyMask
//    ->SetInput(DOUBLE VOL, wc1);
//    ->SetInputMask(DOUBLE VOL, maskNorm);
//    ->GetOutput(DOUBLE VOL);
//


#ifndef __ApplyMask_h
#define __ApplyMask_h

#include "itkImageToImageFilter.h"

namespace itk
{
template< typename TInput, typename TMask, typename TOutput>
class ApplyMask : public ImageToImageFilter< TInput,  TOutput > // This doesn't actually matter, as we will be overriding the output image type in MakeOutput()
{
public:
    /** Standard class typedefs. */
    typedef ApplyMask             Self;
    typedef ImageToImageFilter< TInput,  TOutput > Superclass;
    typedef SmartPointer< Self >        Pointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(ApplyMask, ImageToImageFilter);

    // Input images
    void SetInput(const TInput* image);
    void SetInputMask(const TMask* mask);

    // Output
    TOutput* GetOutput();

protected:
    ApplyMask();
    ~ApplyMask() {}

    /** Does the real work. */
    virtual void GenerateData();

    typename TInput::ConstPointer GetInput();
    typename TMask::ConstPointer GetInputMask();

    /**  Create the Output */
    DataObject::Pointer MakeOutput(unsigned int  idx);

private:
    ApplyMask(const Self &); //purposely not implemented
    void operator=(const Self &);  //purposely not implemented

};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "ApplyMask.hxx"
#endif


#endif // __ApplyMask_h
