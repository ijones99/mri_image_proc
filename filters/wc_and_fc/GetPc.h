
// --------- March 2017, Ian L Jones ---------
//
//GetPc
//    ->SetInputI1(DOUBLE VEC VOL, I1);
//    ->SetInputI0(DOUBLE VEC VOL, I0);
//    ->SetInputPhi(DOUBLE VOL, unwrappedPhase)
//    ->GetOutput(DOUBLE VOL);
//

#ifndef __GetPc_h
#define __GetPc_h

#include "itkImageToImageFilter.h"

namespace itk
{
template< typename TI1, typename TI0, typename TPhi, typename TPc>
class GetPc : public ImageToImageFilter< TI1, TPc> // This doesn't actually matter, as we will be overriding the output image type in MakeOutput()
{
public:
    /** Standard class typedefs. */
    typedef GetPc             Self;
    typedef ImageToImageFilter< TI1,  TPc> Superclass;
    typedef SmartPointer< Self >        Pointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(GetPc, ImageToImageFilter);

    // Input images
    void SetInputI1(const TI1* I1);
    void SetInputI0(const TI0* I0);
    void SetInputPhi(const TPhi* Phi);

    // Output
    TPc* GetOutput();

protected:
    GetPc();
    ~GetPc() {}

    /** Does the real work. */
    virtual void GenerateData();

    typename TI1::ConstPointer GetInputI1();
    typename TI0::ConstPointer GetInputI0();
    typename TPhi::ConstPointer GetInputPhi();


    // Create the Output
    DataObject::Pointer MakeOutput(unsigned int idx);

private:
    GetPc(const Self &); //purposely not implemented
    void operator=(const Self &);  //purposely not implemented

};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATI0N
#include "GetPc.hxx"
#endif


#endif // __GetPc_h
