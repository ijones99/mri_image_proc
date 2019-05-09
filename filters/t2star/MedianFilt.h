// --------- March 2017, Ian L Jones ---------
//
//MedianFilt
//    ->SetInput(DOUBLE VOL, t2StarUnfilt);
//    ->SetFilterSizeX(DOUBLE VAL, CELL_FILTER[0]);
//    ->SetFilterSizeY(DOUBLE VAL, CELL_FILTER[1]);
//    ->GetOutput(DOUBLE VOL);
//

#ifndef __MedianFilt_h
#define __MedianFilt_h

#include "itkImageToImageFilter.h"

namespace itk
{
template< typename TInput, typename TOutput>
class MedianFilt : public ImageToImageFilter< TInput,  TOutput > // This doesn't actually matter, as we will be overriding the output image type in MakeOutput()
{
public:
    /** Standard class typedefs. */
    typedef MedianFilt             Self;
    typedef ImageToImageFilter< TInput,  TOutput > Superclass;
    typedef SmartPointer< Self >        Pointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(MedianFilt, ImageToImageFilter);

    // Input images
    void SetInput(const TInput* imIn);

    itkSetGetDecoratedInputMacro(FilterSizeX, int );
    itkSetGetDecoratedInputMacro(FilterSizeY, int );

    // Output
    TOutput* GetOutput();

protected:
    MedianFilt();
    ~MedianFilt() {}

    /** Does the real work. */
    virtual void GenerateData();

    typename TInput::ConstPointer GetInput();

    /**  Create the Output */
    DataObject::Pointer MakeOutput(unsigned int idx);

private:
    MedianFilt(const Self &); //purposely not implemented
    void operator=(const Self &);  //purposely not implemented

};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "MedianFilt.hxx"
#endif


#endif // __MedianFilt_h
