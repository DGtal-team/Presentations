#!/bin/bash
# Thomas Lewiner, http://www.carva.org/thomas.lewiner
# Matmidia Lab, Dept of Mathematics, PUC-Rio de Janeiro
# September 16th, 2009
#
# pdf thumbnail generator
# using bash and ImageMagick
#
# usage:
# ./pdfthumb.bash file.avi
#
# edit the settings in this file following the comments
#

# pdf rasterization dpi
density=72 ;

# width of a single page (without shading margins)
width=128 ;

# shadow size
shadow=5 ;

# rounded corner size
round=6 ;

# page shift: uncomment here and comment the marked line for a fixed shift between pages
# shift=50;

# automatic page shift to fit the thumbnail in a given width
total_width=300;

# page skip: comment here and uncomment the marked line for skipping every $page_skip page
page_skip=1;

# automatic page skip to keep $total_pages pages
# total_pages=3;


for pdf in "$1" ;
do
# extract rounded images from pdf
    convert -density $density -resize $width -matte -virtual-pixel transparent -channel A -blur 0x$round  -threshold 50% +channel "$pdf" thumbs-%03d.png ;

# add shadows
    for p in thumbs-*.png ;
    do
        convert -page +${shadow}+${shadow} $p -matte \( +clone -background black -shadow 60x${shadow}+${shadow}+${shadow} \) +swap -background none -mosaic ${p/.png/_shadow.png} && rm $p ;
    done ;

# count number of files to compute the offset
    count=0;
    for p in thumbs-*_shadow.png ;
    do
        let "count += 1";
    done
    
# page skip: comment the next line for a fixed total number of pages, or the after next one for a fixed skip
    let "total_pages =$count / $page_skip" ;
#    let "page_skip =$count / $total_pages" ;   if [ $page_skip -lt 1 ] ; then  let "page_skip =1" ; fi ;   let "total_pages =$count / $page_skip" ;

# remove skipped pages
    if [ $page_skip -gt 1 ]  ;
    then
        count=0;
        for p in thumbs-*_shadow.png ;
        do
            if  [ `expr $count % $page_skip` -gt 0 ] ; then rm -f $p ; fi ;
            let "count += 1";
        done      
    fi

# page shift: comment the next line for a fixed shift between pages
    let "shift=($total_width - $width - $shadow)/($total_pages - 1) ";
    echo "$pdf  has $count pages, and pdfthumb will keep $total_pages pages, set to $width px extent each, rounded by $round px, shadowed of $shadow px and shifted by $shift px";

# compose the final image
    offset=0 ;
    cmd="" ;
    for p in thumbs-*_shadow.png ;
    do
        cmd="-page +${offset}+0 $p $cmd" ;
        let "offset += $shift" ;  
    done ;
    convert  $cmd  -background transparent -layers mosaic  "${pdf/.pdf/_thumb.png}" && rm thumbs-*_shadow.png ;

done ;
