
// --------- March 2017, Ian L Jones ---------
//
//GetDeltaB0
//    ->SetInputPhi(DOUBLE VOL, unwrappedPhase);
//    ->SetInputMask(DOUBLE VOL, maskNorm);
//    ->SetDeltaFMax(DOUBLE VAL, DELTA_F_MAX);
//    ->SetGammaH(DOUBLE VAL, GAMMAH_H);
//    ->SetTeVal1(DOUBLE VAL, teVal1);
//    ->SetTeVal2(DOUBLE VAL, teVal2);
//    ->SetTeVal3(DOUBLE VAL, teVal3);
//    ->GetOutput(DOUBLE VOL );
//

#ifndef __GetDeltaB0_txx
#define __GetDeltaB0_txx

#include "GetDeltaB0.h"

#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
//#include "ImageFilterMultipleOutputsDifferentType.h" // add C functions
#include "itkImportImageFilter.h"

namespace itk
{

template< typename TPhi, typename TMask, typename TOut>
GetDeltaB0<TPhi, TMask, TOut>::GetDeltaB0()
{

    this->SetNumberOfRequiredInputs(2);

    this->SetNthOutput( 0, this->MakeOutput(0) );

}

template< typename TPhi, typename TMask, typename TOut>
void GetDeltaB0<TPhi, TMask, TOut>::SetInputPhi(const TPhi* phi)
{
    this->SetNthInput(0, const_cast<TPhi*>(phi));
}

template< typename TPhi, typename TMask, typename TOut>
void GetDeltaB0<TPhi, TMask, TOut>::SetInputMask(const TMask* mask)
{
    this->SetNthInput(1, const_cast<TMask*>(mask));
}

template< typename TPhi, typename TMask, typename TOut>
typename TPhi::ConstPointer GetDeltaB0<TPhi, TMask, TOut>::GetInputPhi()
{
    return static_cast< const TPhi * >
           ( this->ProcessObject::GetInput(0) );
}
template< typename TPhi, typename TMask, typename TOut>
typename TMask::ConstPointer GetDeltaB0<TPhi, TMask, TOut>::GetInputMask()
{
    return static_cast< const TMask * >
           ( this->ProcessObject::GetInput(1) );
}


template< typename TPhi, typename TMask, typename TOut>
void GetDeltaB0<TPhi, TMask, TOut>::GenerateData()
{
    /*


     *  IN:
    *      phi (vector, double)
    *      mask (scalar, unsigned char)
    *      TE (vector, double)
    *  OUT:
    *      deltaB0 (scalar, double)



     */
    const double DELTA_F_MAX = this->GetDeltaFMax();      // max delta freq [Hz]
    const double GAMMAH_H= this->GetGammaH();    //1H gyromagnetic constant [Hz/T]
    const double teVal1 = this->GetTeVal1();
    const double teVal2 = this->GetTeVal2();
    const double teVal3 = this->GetTeVal3();

    const double teVal[] = {teVal1,  teVal2,  teVal3};
    const double PI = 3.141592653;

    typedef itk::Image< double, 2 >       PixelTypePhi;
    typedef itk::Image< PixelTypePhi, 2 >    ImageTypePhi;

    typedef double PixelTypeFloat;

    typename TPhi::ConstPointer phi = this->GetInputPhi();
    typename TMask::ConstPointer mask = this->GetInputMask();
    typename TOut::Pointer output = this->GetOutput();

    // allocate space for output
    typename TPhi::RegionType region = phi->GetLargestPossibleRegion();
    typename TPhi::SizeType regionSize = region.GetSize();
    output->SetRegions(regionSize);
    output->Allocate();
    /*
    *  mask = mask/255;
     *  delta_t = (TE(3) - TE(1))*1e-3; % [sec]
     *  delta_f = phi./(2*pi*delta_t);
     *  delta_f(abs(delta_f) > delta_f_max) = NaN;
     *  delta_f = delta_f.* mask;
     *  deltaB0 = delta_f./(GammaH_Hz);  % deltaB0 in T
     *  deltaB0 = 1e6*deltaB0.*mask; % deltaB0 in microT
    */
    for (int i=0; i<regionSize[0]; i++)
    {
        for (int j=0; j<regionSize[1]; j++)
        {

            for (int m=0; m<regionSize[2]; m++)
            {
                // get phi value
                typename TPhi::IndexType Pixel1 ;
                Pixel1[0] = i;
                Pixel1[1] = j;
                Pixel1[2] = m;
                typename TPhi::PixelType valPhi;
                valPhi = phi->GetPixel(Pixel1);

                // get mask value
                typename TMask::PixelType valMask;
                valMask = mask->GetPixel(Pixel1);

                typename TOut::PixelType valDeltaB0;

                //double valMaskFloat=(double)(valMask)/255;
                double delta_t = (teVal[2] - teVal[0])*1e-3;
                double delta_f = valPhi/(2*PI*delta_t);
                if (abs(delta_f) > DELTA_F_MAX)
                {
                    delta_f = std::numeric_limits<unsigned char >::quiet_NaN();
                }
                delta_f = delta_f* valMask;
                double deltaB0 = delta_f/GAMMAH_H;  // deltaB0 in T
                deltaB0 = 1e6*deltaB0*valMask; // deltaB0 in microT
                output->SetPixel(Pixel1, deltaB0);
            }

        }
    }


}
template< typename TPhi, typename TMask, typename TOut> // why is TImage a class when there is only one type and a typename
// when there are multiple image types?
DataObject::Pointer GetDeltaB0<TPhi, TMask, TOut>::
MakeOutput(unsigned int idx)
{
    DataObject::Pointer output;

    switch ( idx )
    {
    case 0:
        output = ( TOut::New() ).GetPointer();
        break;
    default:
        std::cerr << "No output " << idx << std::endl;
        output = NULL;
        break;
    }
    return output.GetPointer();
}

template< typename TPhi, typename TMask, typename TOut>
TOut* GetDeltaB0<TPhi, TMask, TOut>::GetOutput()
{
    return dynamic_cast< TOut * >(this->ProcessObject::GetOutput(0) );
}


}// end namespace


#endif
