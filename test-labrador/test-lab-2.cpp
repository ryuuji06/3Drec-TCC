/** OpenMVG TEST 15
 * 
 **/


/* ================================
 *  EXAMPLES
 * ================================
 * 
 * 
 * ================================
 *  COMMENTS
 * ================================
 * 
 * // testar MVE sem thumbnail, ver se muda o resultado
 * 
 */



#include "openMVG/image/image_io.hpp"

// FEATURE DETECTION AND DESCRIPTION
#include "openMVG/features/feature.hpp"
#include "openMVG/features/image_describer_akaze_io.hpp"
#include "openMVG/features/sift/SIFT_Anatomy_Image_Describer_io.hpp"
#include "openMVG/features/regions_factory_io.hpp"

#include <iostream>
#include <string>
#include <utility>
#include <sys/time.h>

using namespace openMVG::image;
using namespace openMVG::features;
using namespace std;


// ==========================================================
// ENUMERATE to choose options
// ==========================================================
EDESCRIBER_PRESET stringToEnum(const uint8_t i)
{
  EDESCRIBER_PRESET preset;
  if (i==0)
    preset = NORMAL_PRESET;
  else
  if (i==1)
    preset = HIGH_PRESET;
  else
  if (i==2)
    preset = ULTRA_PRESET;
  else
    preset = EDESCRIBER_PRESET(-1);
  return preset;
}



int main(int argc, char **argv)
{
	// =================================================================
	//   ( 0 )   S E T   O P T I O N S
	// =================================================================
	
	string sImageFilename = "kermit.jpg";
	string sView_filename = "edited.jpg";
	uint8_t iDescriberType = 0;
	uint8_t iDescPreset = 0;
	
	// TIMERS
	struct timeval start, end;
	long mtime, seconds, useconds;
	
	gettimeofday(&start,NULL);
	

	// =================================================================
	//   ( 1 )  FEATURE DETECTION AND DESCRIPTION
	// =================================================================

	// (1.1) INITIALIZE DESCRIBER
	
	unique_ptr<Image_describer> image_describer;
	
	switch (iDescriberType)
	{
		case 0:
			image_describer.reset(new SIFT_Anatomy_Image_describer(SIFT_Anatomy_Image_describer::Params()));
		break;
		case 1:
			image_describer = AKAZE_Image_describer::create(AKAZE_Image_describer::Params(AKAZE::Params(), AKAZE_MSURF));
		break;
	}
	
	image_describer->Set_configuration_preset(stringToEnum(iDescPreset));
    
    
    // ==========================================================
	// (1.2) UNDISTORT, SAVE IMAGE AND DETECT FEATURES
	
	Image<RGBColor> imageRGB;
	Image<unsigned char> imageGray;
	
	ReadImage(sImageFilename.c_str(), &imageRGB);
	WriteImage(sView_filename.c_str(), imageRGB);
	
	
	ConvertPixelType(imageRGB, &imageGray);
	
	auto regions = image_describer->Describe(imageGray);
	
	image_describer->Save(regions.get(), "image.feat","image.desc");
	
	gettimeofday(&end,NULL);
	seconds  = end.tv_sec  - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	mtime = (1000000*seconds + useconds)/1000;
	
	// TEST PRINTS
	cout << "Regions: " << regions->RegionCount()
		 << "\tTime: " << mtime << " ms" << endl;
	
	return EXIT_SUCCESS;
    
}


