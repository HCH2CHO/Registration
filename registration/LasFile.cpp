//
// Created by zzw on 8/20/20.
//

#include "LasFile.h"
#include <iomanip>
#include <sstream>
#include <istream>
#include <ostream>
#include <lasreader.hpp>
#include <laswriter.hpp>

LasFile::LasFile() {

}

LasFile::~LasFile() {

}

void LasFile::ReadLasHeader(std::string strLasPath, pcl::PointCloud<pcl::PointXYZ>::Ptr &cloud) {
    //打开las文件
    LASreadOpener lasreadopener;
    lasreadopener.set_file_name(strLasPath.c_str());
    LASreader *pLasReader = lasreadopener.open();
    if (pLasReader == NULL) {
        return;
    }

    size_t nPointCount = pLasReader->header.number_of_point_records;
    cloud->resize(nPointCount);
    cloud->width = 1;
    cloud->height = nPointCount;
    cloud->is_dense = false;

    //double minX = pLasReader->get_min_x();
    //double minY = pLasReader->get_min_y();
    //double maxX = pLasReader->get_max_x();
    //double maxY = pLasReader->get_max_y();

    //读取.las文件的版本号
    unsigned char Vmajor = pLasReader->header.version_major;
    unsigned char Vminor = pLasReader->header.version_minor;
    printf("LasVersion: %d.%d\n", Vmajor, Vminor);

    //变长记录区vlrs描述
    char *ds = (char *) pLasReader->header.vlrs->description;
    if (ds != nullptr) {
        printf("vlrs description: %s\n", ds);
    }

    //变长记录区id
    U16 Nofkeys2 = pLasReader->header.vlrs->record_id;
    printf("vlrs_id: %d\n", Nofkeys2);

    //geokeys个数
    U16 Nofkeys = pLasReader->header.vlr_geo_keys->number_of_keys;//
    printf("number_of_keys: %d\n", Nofkeys);

    //试着读取wkt失败，因该las文件版本为：1.2，只有最新得1.4版本才加入了wkt
    char *WKT = pLasReader->header.vlr_geo_ogc_wkt;
    printf("%s\n", WKT);

    //顺序读取Geokeys
    int numof_keys = pLasReader->header.vlr_geo_keys->number_of_keys;
    for (int i = 0; i < numof_keys; i++) {
        //顺序读取GeoKey内容
        U16 id = (pLasReader->header.vlr_geo_key_entries + i)->key_id;
        U16 Location = (pLasReader->header.vlr_geo_key_entries + i)->tiff_tag_location;
        U16 count = (pLasReader->header.vlr_geo_key_entries + i)->count;
        U16 value_offset = (pLasReader->header.vlr_geo_key_entries + i)->value_offset;
        //vlr_geo_double_params
        F64 value;
        if (Location == 34736) {
            value = pLasReader->header.vlr_geo_double_params[value_offset];
            printf("keysID:%d ,Location:%d,Count: %d ,Value_offset:%d ,Value:%f \n", id, Location, count, value_offset,
                   value);
        } else if (Location == 34737) {//vlr_geo_ascii_params
            CHAR value_char[32];
            for (int j = 0; j < count; j++) {
                value_char[j] = pLasReader->header.vlr_geo_ascii_params[value_offset + j];
            }
            printf("keysID:%d ,Location:%d,Count: %d ,Value_offset:%d ,ValueChar:%s \n", id, Location, count,
                   value_offset, value_char);
        } else {
            printf("keysID:%d ,Location:%d,Count: %d ,Value_offset:%d\n", id, Location, count, value_offset);
        }
    }

    //double dMinx, dMiny, dMaxX, dMaxY;
    //pLasreader->inside_rectangle(dMinx, dMiny, dMaxX, dMaxY);

    //读取点云坐标
    int i = 0;
    while (pLasReader->read_point()) {
        if (i++ == 1500000) {
            break;
        }
        cloud->push_back(
                pcl::PointXYZ(pLasReader->point.get_x(), pLasReader->point.get_y(), pLasReader->point.get_z()));

        pLasReader->point.get_R();
    }
    pLasReader->close();
}


void LasFile::ReadLas(std::string strLasPath, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud) {
    LASreadOpener lasreadopener;
    lasreadopener.set_file_name(strLasPath.c_str());
    if (!lasreadopener.active()) {
        return;
    }
    LASreader *pLasreader = lasreadopener.open();
    if (pLasreader == nullptr) {
        return;
    }

    //cloud->resize(pLasreader->header.number_of_point_records);

    double x;
    double y;
    double z;
    int r;
    int g;
    int b;
    int intensity;
    int return_number;
    int number_of_returns;
    int scan_angle_rank;
    double gpsTime;

//    double x0;
//    double y0;
//    double z0;
//    pLasreader->read_point();
//    x0 = pLasreader->point.get_x();
//    y0 = pLasreader->point.get_y();
//    z0 = pLasreader->point.get_z();

    while (pLasreader->read_point()) {
        x = pLasreader->point.get_x();
        y = pLasreader->point.get_y();
        z = pLasreader->point.get_z();
        //r = pLasreader->point.get_R();
        //g = pLasreader->point.get_G();
        //b = pLasreader->point.get_B();
        //intensity = pLasreader->point.get_intensity(); //回波强度
        //return_number = pLasreader->point.get_return_number(); //第几次回波
        //number_of_returns = pLasreader->point.get_number_of_returns();//回波次数
        //scan_angle_rank = pLasreader->point.get_scan_angle_rank(); //扫描角
        //gpsTime = pLasreader->point.get_gps_time();

        cloud->push_back(pcl::PointXYZ(x, y, z));
    }

    pLasreader->close();
}

