// --------- March 2017, Ian L Jones ---------
//
//GetPc
//    ->SetInputI1(DOUBLE VEC VOL, I1);
//    ->SetInputI0(DOUBLE VEC VOL, I0);
//    ->SetInputPhi(DOUBLE VOL, unwrappedPhase)
//    ->GetOutput(DOUBLE VOL);
//

#ifndef __GetPc_txx
#define __GetPc_txx

#include "GetPc.h"

#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
//#include "ImageFilterMultipleOutputsDifferentType.h" // add C functions
#include "itkImportImageFilter.h"

namespace itk
{

template< typename TI1, typename TI0, typename TPhi, typename TPc>
GetPc<TI1, TI0, TPhi, TPc>::GetPc()
{

    this->SetNumberOfRequiredInputs(3);

    this->SetNthOutput( 0, this->MakeOutput(0) );

}

template< typename TI1, typename TI0, typename TPhi, typename TPc>
void GetPc<TI1, TI0, TPhi, TPc>::SetInputI1(const TI1* I1)
{
    this->SetNthInput(0, const_cast<TI1*>(I1));
}

template< typename TI1, typename TI0, typename TPhi, typename TPc>
void GetPc<TI1, TI0, TPhi, TPc>::SetInputI0(const TI0* I0)
{
    this->SetNthInput(1, const_cast<TI0*>(I0));
}

template< typename TI1, typename TI0, typename TPhi, typename TPc>
void GetPc<TI1, TI0, TPhi, TPc>::SetInputPhi(const TPhi* Phi)
{
    this->SetNthInput(2, const_cast<TPhi*>(Phi));
}

template< typename TI1, typename TI0, typename TPhi, typename TPc>
typename TI1::ConstPointer GetPc<TI1, TI0, TPhi, TPc>::GetInputI1()
{
    return static_cast< const TI1 * >
           ( this->ProcessObject::GetInput(0) );
}
template< typename TI1, typename TI0, typename TPhi, typename TPc>
typename TI0::ConstPointer GetPc<TI1, TI0, TPhi, TPc>::GetInputI0()
{
    return static_cast< const TI0 * >
           ( this->ProcessObject::GetInput(1) );
}
template< typename TI1, typename TI0, typename TPhi, typename TPc>
typename TPhi::ConstPointer GetPc<TI1, TI0, TPhi, TPc>::GetInputPhi()
{
    return static_cast< const TPhi * >
           ( this->ProcessObject::GetInput(2) );
}

template< typename TI1, typename TI0, typename TPhi, typename TPc>
void GetPc<TI1, TI0, TPhi, TPc>::GenerateData()
{
    typename TI1::ConstPointer i1 = this->GetInputI1();
    typename TI0::ConstPointer i0 = this->GetInputI0();
    typename TPhi::ConstPointer phi = this->GetInputPhi();
    typename TPc::Pointer output = this->GetOutput();


    // allocate space for output
    typename TPhi::RegionType region = phi->GetLargestPossibleRegion();
    typename TPhi::SizeType regionSize = region.GetSize();
    output->SetRegions(regionSize);
    output->Allocate();
    for (int i=0; i< regionSize[0]; i++)
    {
        for (int j=0; j< regionSize[1]; j++)
        {

            for (int m=0; m<regionSize[2]; m++)
            {
                /*
                % Algebraic solution with phase correction.

                I1_prime = I1.*conj(I0)./abs(I0);

                pc = cos(angle(I1_prime.*exp(-Im*phi)));*/


                // get phi value
                typename TI1::IndexType Pixel1 ;
                Pixel1[0] = i;
                Pixel1[1] = j;
                Pixel1[2] = m;
                typename TI1::PixelType valI1;
                valI1 = i1->GetPixel(Pixel1);
                typename TI0::PixelType valI0;
                valI0 = i0->GetPixel(Pixel1);
                typename TPhi::PixelType valPhi;
                valPhi= phi->GetPixel(Pixel1);

                //I1_prime = I1.*conj(I0)./abs(I0);
                //  conj(X) = REAL(X) - i*IMAG(X).
                //  abs(im) = sqrt(x^2+y^2)
                //  complex # mult: (x+yi)(u+vi) = (xu-yv)+(xv+yu)i
                //typename TI0::PixelType conjI0;
                //std::vector <float> conjI0(2);
                typename TI0::PixelType conjI0;
                conjI0[0] = valI0[0];
                conjI0[1] = -valI0[1];
                typename TI0::PixelType I1Prime;

//                                I1Prime[0] = (valI1[0]*conjI0[0]-valI1[1]*conjI0[1])/round(sqrt(pow(valI0[0],2)+pow(valI0[1],2)));
//                I1Prime[1] = (valI1[0]*conjI0[1]+valI1[1]*conjI0[0])/round(sqrt(pow(valI0[0],2)+pow(valI0[1],2)));

                if (valI1[0]==0)
                    valI1[0] = 0;
                if (valI1[1]==0)
                    valI1[1] = 0;
                if (conjI0[0]==0)
                    conjI0[0] = 0;
                if (conjI0[1]==0)
                    conjI0[1] = 0;
                if (valI0[0]==0)
                    valI0[0] = 0;
                if (valI0[1]==0)
                    valI0[1] = 0;


                I1Prime[0] = (valI1[0]*conjI0[0]-valI1[1]*conjI0[1])/(std::sqrt(std::pow(valI0[0],2)+std::pow(valI0[1],2)));
                I1Prime[1] = (valI1[0]*conjI0[1]+valI1[1]*conjI0[0])/(std::sqrt(std::pow(valI0[0],2)+std::pow(valI0[1],2)));

                if (I1Prime[0]==0)
                    I1Prime[0] = 0;
                if (I1Prime[1]==0)
                    I1Prime[1] = 0;


                //pc = cos(angle(I1_prime.*exp(-Im*phi)));
                //  -Im = 0 - 1*i


                // COMPUTE: exp(-Im*phi)
                // complex Z=X+i*Y, exp(Z) = exp(X)*(COS(Y)+i*SIN(Y))
                typename TI1::PixelType expPhi;
                expPhi[0] = std::exp(0)*std::cos(-valPhi);
                expPhi[1] = std::exp(0)*std::sin(-valPhi);

                // COMPUTE: I1_prime.*expPhi
                //  complex # mult: (x+yi)(u+vi) = (xu-yv)+(xv+yu)i
                typename TI1::PixelType angleIn;
                float x = I1Prime[0];
                float y = I1Prime[1];
                float u = expPhi[0];
                float v = expPhi[1];
                angleIn[0] = (x*u-y*v);
                angleIn[1] = (x*v+y*u);

                if (angleIn[0]==0)
                    angleIn[0] = 0;
                if (angleIn[1]==0)
                    angleIn[1] = 0;

                // COMPUTE: pcOut = cos(angle(angleIn))
                //  angle = atan2(imag(h), real(h));
                typename TPc::PixelType pcOut;
                pcOut = std::cos(std::atan2(angleIn[1],angleIn[0]));

                if (i==114-1 && j == 47-1 && m == 21-1)
                {
                    std::cout << "\n\n\nPC INFO>>> "<<  valI0[0] << ", " << valI0[1] << ", "
                    << valI1[0]<< ", " << valI1[0] << ", " << I1Prime[0]  << ", " << I1Prime[1] << ", " << pcOut << std::endl;

                }


                output->SetPixel(Pixel1, pcOut);
            }
        }
    }


}
template< typename TI1, typename TI0, typename TPhi, typename TPc> // why is TImage a class when there is only one type and a typename
// when there are multiple image types?
DataObject::Pointer GetPc<TI1, TI0, TPhi, TPc>::
MakeOutput(unsigned int idx)
{
    DataObject::Pointer output;

    switch ( idx )
    {
    case 0:
        output = ( TPc::New() ).GetPointer();
        break;
    default:
        std::cerr << "No output " << idx << std::endl;
        output = NULL;
        break;
    }
    return output.GetPointer();
}

template< typename TI1, typename TI0, typename TPhi, typename TPc>
TPc* GetPc<TI1, TI0, TPhi, TPc>::GetOutput()
{
    return dynamic_cast< TPc * >(this->ProcessObject::GetOutput(0) );
}


}// end namespace


#endif
