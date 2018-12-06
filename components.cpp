#include "root.hpp"

void process(const char * imsname, const char * imdname)
{
  Mat ims = imread(imsname, 0);
  if (!ims.data) {
    cerr << "Could not open or find the image corresponding to " << imsname << "\n" << endl;
    exit(EXIT_FAILURE);
  }

  Mat imd;
  ims.copyTo(imd);

  int N = connectedComponents(ims, imd, 8, CV_32S); // Needs at least OpenCV 3.0

  cout << N << " components detected" << endl;

  imwrite(imdname, imd);
}

void usage(const char *s)
{
  cerr << "Usage: " << s << " input_img_path" << endl;
  exit(EXIT_FAILURE);
}

#define param 2
int main(int argc, char * argv[])
{
  if(argc != (param+1))
    usage(argv[0]);
  process(argv[1], argv[2]);
  return EXIT_SUCCESS;
}
