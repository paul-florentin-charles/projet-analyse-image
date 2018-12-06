#include "root.hpp"

void process(const char * imsname, const char * imdname, int conversion_type)
{
  Mat ims = imread(imsname);
  if (!ims.data) {
    cerr << "Could not open or find the image corresponding to " << imsname << "\n" << endl;
    exit(EXIT_FAILURE);
  }

  if (ims.channels() != 3 or ims.depth() != CV_8U) {
    cerr << "Conversion only works for CV_8UC3 images" << endl;
    exit(EXIT_FAILURE);
  }

  Mat imd;
  ims.copyTo(imd);
  cvtColor(ims, imd, conversion_type);

  imwrite(imdname, imd);
}

void usage(const char * s)
{
  cerr << "Usage: " << s << " input_img_path output_img_name conversion_type"<< endl;
  exit(EXIT_FAILURE);
}

#define param 3
int main(int argc, char * argv[])
{
  if (argc != (param + 1))
    usage(argv[0]);
  process(argv[1], argv[2], atoi(argv[3]));
  return EXIT_SUCCESS;
}
