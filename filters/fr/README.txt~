SUMMARY:
	Used to get fat and water content ratio:

	Wr = wc./(fc+wc)*100;
	fr = fc./(fc+wc)*100; 


USAGE:
	// Get fr: fr = fc./(fc+wc)*100;
    	typedef itk::GetContentRatio<ImageTypeDoubleVol, ImageTypeDoubleVol, ImageTypeDoubleVol> FilterTypeFr;
	FilterTypeFr::Pointer frFilter = FilterTypeFr::New();
    	frFilter->SetInput1(fcMask);
    	frFilter->SetInput2(wcMask);
    	frFilter->Update();
    	typename ImageTypeDoubleVol::Pointer fr = frFilter->GetOutput();

	// Get wc: wc = fc./(fc+wc)*100;
 	typedef itk::GetContentRatio<ImageTypeDoubleVol, ImageTypeDoubleVol, ImageTypeDoubleVol> FilterTypeWr;
    	FilterTypeWr::Pointer wrFilter = FilterTypeWr::New();
    	wrFilter->SetInput1(wcMask);
    	wrFilter->SetInput2(fcMask);
    	wrFilter->Update();
    	typename ImageTypeDoubleVol::Pointer wr = wrFilter->GetOutput();
	
INPUT: 
	"wcMask", "fcMask"

OUTPUT: 

	"wr" or "fr", depending on input order

