if [ $# -ne 1 ]
then
  echo "Usage : ./process.sh path_to_image"
  exit
elif [ ! -s "$1" ]
then
  echo "File $1 doesn't exist or is empty"
  exit
fi

make all

echo -e "\nList of possible conversions"
echo ".  0 : Stay in RGB"
echo ".  6 : RGB to Gray"
echo ". 36 : RGB to YCrCb"
echo ". 40 : RGB to HSV"
echo ". 44 : RGB to Lab"
echo ". 50 : RGB to Luv"
echo -e ". 52 : RGB to HLS\n"

read -p '$ Conversion type : ' cvt_type

if [ $cvt_type -eq 0 ]
then
  type="rgb"
elif [ $cvt_type -eq 6 ]
then
  type="gray"
elif [ $cvt_type -eq 36 ]
then
  type="ycrcb"
elif [ $cvt_type -eq 40 ]
then
  type="hsv"
elif [ $cvt_type -eq 44 ]
then
  type="lab"
elif [ $cvt_type -eq 50 ]
then
  type="luv"
elif [ $cvt_type -eq 52 ]
then
  type="hls"
else
  type="unknown"
fi

name=$type"_"$(basename -- $1)

## Conversion ##

./convert $1 $name $cvt_type
eog $name

echo -e '\nWe will proceed to a kmeans of this image'
echo 'Please type a integer greater or equal to 2'
read -p '$ Number of clusters : ' n_clusters

while [ $n_clusters -lt 0 ]
do
  echo 'Please type a positive integer'
  read -p '$ Number of clusters : ' n_clusters
done

## Kmeans ##

./kmeans $n_clusters $name "kmeaned_"$name
name="kmeaned_"$name
eog $name

## Otsu ##

echo -e '\nWe will proceed to apply otsu to this image'
./otsu $name "otsu_"$name
name="otsu_"$name
eog $name

## Contour detection ##

echo -e '\nLet'\''s apply contour detection'
./contours $name 'contoured_'$name
eog 'contoured_'$name

labname="labeled_"$name
colname="colored_"$name

## First labeling ##

echo -e '\nLabeling of the components'
./labeling-color $name $labname $colname
eog $labname
eog $colname

## Filtering ##

echo -e '\nFiltering'
./filter $labname 'filtered_'$labname
name='filtered_'$labname
eog $name

## Second labeling ##

echo -e '\nFinally, another labeling of the components'
./labeling-color $name $labname $colname
eog $labname
eog $colname
