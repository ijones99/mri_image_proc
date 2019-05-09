/*=========================================================================
 *
 *  Ian L Jones 30.01.2017
 *  IN: vector (complex number representation)
 *  OUT: scalar
 *
 *=========================================================================*/

#ifndef __CreateWrappedPhaseDegree_h
#define __CreateWrappedPhaseDegree_h

#include "itkImageToImageFilter.h"

namespace itk
{
template< typename TInput, typename TOutput>
class CreateWrappedPhaseDegree : public ImageToImageFilter< TInput,  TOutput > // This doesn't actually matter, as we will be overriding the output image type in MakeOutput()
{
public:
    /** Standard class typedefs. */
    typedef CreateWrappedPhaseDegree             Self;
    typedef ImageToImageFilter< TInput,  TOutput > Superclass;
    typedef SmartPointer< Self >        Pointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(CreateWrappedPhaseDegree, ImageToImageFilter);

    // Input images
    void SetInput(const TInput* wrapped_phase);

    // Output
    TOutput* GetOutput();

protected:
    CreateWrappedPhaseDegree();
    ~CreateWrappedPhaseDegree() {}

    /** Does the real work. */
    virtual void GenerateData();

    typename TInput
    ::ConstPointer GetInput();

    /**  Create the Output */
    DataObject::Pointer MakeOutput(unsigned int idx);

private:
    CreateWrappedPhaseDegree(const Self &); //purposely not implemented
    void operator=(const Self &);  //purposely not implemented

};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "CreateWrappedPhaseDegree.hxx"
#endif


#endif // __CreateWrappedPhaseDegree_h
