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

  vector<unsigned int> surfaces(256, 0);
  for (int i = 0; i < ims.rows; i++) {
    for (int j = 0; j < ims.cols; j++) {
      if (ims.at<unsigned char>(i, j)) surfaces[ims.at<unsigned char>(i, j)]++;
    }
  }

  for (int i = 0; i < 256; i++) {
    cout << surfaces[i] << endl;
  }

  int max_index = distance(surfaces.begin(), max_element(surfaces.begin(), surfaces.end()));

  unsigned int max_surface = surfaces[max_index];
  float M = 1.f / 5;

  for (int i = 0; i < ims.rows; i++) {
    for (int j = 0; j < ims.cols; j++) {
      if (surfaces[ims.at<unsigned char>(i, j)] < M * max_surface)
        imd.at<unsigned char>(i, j) = 0;
    }
  }

  imwrite(imdname, imd);
}

void usage(const char *s)
{
  cerr << "Usage: " << s << " input_img_path output_img_name" << endl;
  exit(EXIT_FAILURE);
}

#define param 2
int main(int argc, char * argv[])
{
  if(argc != (param + 1))
    usage(argv[0]);
  process(argv[1], argv[2]);
  return EXIT_SUCCESS;
}
