#include "root.hpp"

RNG rng(12345);

int _find(int p, int * roots)
{
  while (roots[p] != p)
    p = roots[p];
  return p;
}

int _union(int r0, int r1, int * roots)
{
  if (r0 == r1) return r0;
  if (r0 == -1) return r1;
  if (r1 == -1) return r0;
  if (r0 <  r1) {
    roots[r1] = r0;
    return r0;
  } else {
    roots[r0] = r1;
    return r1;
  }
}

int _add(int p, int r, int * roots)
{
  if (r == -1)
    roots[p] = p;
  else
    roots[p] = r;
  return roots[p];
}

void process(const char * imsname, const char * regname, const char * colorname)
{
  Mat ims = imread(imsname);
  if (!ims.data) {
    cerr << "Could not open or find the image corresponding to " << imsname << "\n" << endl;
    exit(EXIT_FAILURE);
  }

  if (ims.channels() == 3) cvtColor(ims, ims, CV_BGR2GRAY);

  int * roots = new int[ims.total()];
  int rows = ims.rows, cols = ims.cols;
  int p = 0, r;
  unsigned char * ps = ims.data;

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      r = -1;
      if (j > 0 && (*(ps - 1) == (*ps)))
        r = _union( _find(p - 1, roots), r, roots);
      if ((i>0 && j>0) && (*(ps-1-cols) == (*ps)))
        r = _union( _find(p-1-cols, roots), r, roots);
      if (i>0 && (*(ps-cols) == (*ps)))
        r = _union(_find(p-cols, roots), r, roots);
      if ((j<(cols-1) && i>0) && (*(ps+1-cols) == (*ps)))
        r = _union(_find(p+1-cols, roots), r, roots);

      r = _add(p, r, roots);

      p++;
      ps++;
    }
  }

  for (p = 0; p < rows * cols; p++) {
    roots[p] = _find(p, roots);
  }

  int l = 0;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int p = i * cols + j;
      if (roots[p] == p)
        roots[p] = l++;
      else
	     roots[p] = roots[roots[p]];
    }
  }

  cout << "labeling: " << l << " components detected" << endl;

  Mat color, reg;
  ims.copyTo(color);
  ims.copyTo(reg);

  int labels[l];
  unsigned int cpt = 0, factor;
  if (l < 50)
    factor = 5;
  else if (l < 80)
    factor = 3;
  else
    factor = 1;

  for (int i = 0; i < rows; i++)  {
    for (int j = 0; j < cols; j++) {
      int r = roots[j + i * cols];
      reg.at<unsigned char>(i,j) = (r * factor)%256; // Redundant labels if l > 256
      if (find(labels, labels + cpt, r) == labels + cpt) {
        labels[cpt] = r;
        cpt++;
      }
    }
  }

  imwrite(regname, reg);

  cvtColor(color, color, CV_GRAY2BGR);
  cvtColor(color, color, CV_BGR2HSV);

  float step = 180 / (l + 1);
  float hues[l];
  for (int i = 0; i < l; i++) {
    hues[i] = (i + 1) * step;
  }

  for (int i = 0; i < rows; i++)  {
    for (int j = 0; j < cols; j++) {
      int r = roots[j + i * cols];
      if (r) {
        unsigned int index = distance(labels, find(labels, labels + l, r));
        color.at<Vec3b>(i,j) = Vec3b(hues[index], 255, 255);
      }
    }
  }

  cvtColor(color, color, CV_HSV2BGR);

  imwrite(colorname, color);

  delete [] roots;
}

void usage(const char * s)
{
  cerr << "Usage: " << s << " input_img_path labeled_img_name colored_img_name" << endl;
  exit(EXIT_FAILURE);
}

#define param 3
int main(int argc, char * argv[])
{
  if (argc != (param + 1))
    usage(argv[0]);
  process(argv[1], argv[2], argv[3]);
  return EXIT_SUCCESS;
}
