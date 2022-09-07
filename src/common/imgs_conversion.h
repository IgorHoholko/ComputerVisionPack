//
// Created by Игорь Хохолко on 25.08.22.
//

#ifndef COMPUTERVISIONPACK_IMGS_CONVERSION_H
#define COMPUTERVISIONPACK_IMGS_CONVERSION_H

#include "opencv2/imgproc.hpp"
#include <QGraphicsPixmapItem>


namespace cvp{

    inline cv::Mat qPixmapToMat(const QPixmap& pixmap){
        QImage image = pixmap.toImage();
        image = image.convertToFormat(QImage::Format_RGB888);
        return cv::Mat(image.height(),image.width(),CV_8UC3,image.bits(),image.bytesPerLine());
    }

    inline QPixmap matToQPixmap(cv::Mat mat){
        QImage image_blurred(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return QPixmap::fromImage(image_blurred);
    }

}

#endif//COMPUTERVISIONPACK_IMGS_CONVERSION_H
