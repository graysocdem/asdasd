// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <cstdio>
#include <vector>
#include <typeinfo>
#include <cstdint>

#include <boost/polygon/voronoi.hpp>
using boost::polygon::voronoi_builder;
using boost::polygon::voronoi_diagram;
using boost::polygon::x;
using boost::polygon::y;
using boost::polygon::low;
using boost::polygon::high;

struct Point {
    int a;
    int b;
    Point(int x, int y) : a(x), b(y) {}
};

struct Segment {
    Point p0;
    Point p1;
    Segment(int x1, int y1, int x2, int y2) : p0(x1, y1), p1(x2, y2) {}
};

struct TransferPoint {
    int x;
    int y;
};

struct TransferPointArray {
    TransferPoint* arrayPtr;
    std::size_t length;
};


namespace boost {
    namespace polygon {

        template <>
        struct geometry_concept<Point> {
            typedef point_concept type;
        };

        template <>
        struct point_traits<Point> {
            typedef int coordinate_type;

            static inline coordinate_type get(
                const Point& point, orientation_2d orient) {
                return (orient == HORIZONTAL) ? point.a : point.b;
            }
        };

        template <>
        struct geometry_concept<Segment> {
            typedef segment_concept type;
        };

        template <>
        struct segment_traits<Segment> {
            typedef int coordinate_type;
            typedef Point point_type;

            static inline point_type get(const Segment& segment, direction_1d dir) {
                return dir.to_int() ? segment.p1 : segment.p0;
            }
        };
    }  // polygon
}  // boost


std::vector<TransferPoint> get_vertices(const voronoi_diagram<double>& vd) {
    std::vector<TransferPoint> collect;
    for (voronoi_diagram<double>::const_vertex_iterator it =
        vd.vertices().begin(); it != vd.vertices().end(); ++it) {
        TransferPoint temp{ int(it->x()), int(it->y()) };
        collect.push_back(temp);
    }
    return collect;
}

#define EXPORTED_METHOD extern "C" __declspec(dllexport)

EXPORTED_METHOD
TransferPointArray get_voronoi_vertices() {
    TransferPointArray result;

    std::vector<Point> points{};
    std::vector<Segment> segments{};
    segments.push_back(Segment(-4, 5, 5, -1));
    segments.push_back(Segment(3, -11, 13, -1));

    // Construction of the Voronoi Diagram.
    voronoi_diagram<double> vd{};
    construct_voronoi(points.begin(), points.end(),
        segments.begin(), segments.end(),
        &vd);

    // Get vector of pointsT
    std::vector<TransferPoint> vertixVector = get_vertices(vd);

    //REMEMBER TO DELETE
    TransferPoint* arr = new TransferPoint[vertixVector.size()];
    std::copy(vertixVector.begin(), vertixVector.end(), arr);
    result.arrayPtr = arr;
    result.length = vertixVector.size();



    return result;
}



//TransferPoint* get_voronoi_vertices(int* size) {
//    std::vector<Point> points;
//    std::vector<Segment> segments;
//    segments.push_back(Segment(-4, 5, 5, -1));
//    segments.push_back(Segment(3, -11, 13, -1));
//
//    // Construction of the Voronoi Diagram.
//    voronoi_diagram<double> vd;
//    construct_voronoi(points.begin(), points.end(),
//        segments.begin(), segments.end(),
//        &vd);
//
//    int vertixAmnt{ 0 };
//    for (voronoi_diagram<double>::const_vertex_iterator it =
//        vd.vertices().begin(); it != vd.vertices().end(); ++it) {
//        vertixAmnt++;
//    }
//    *size = vertixAmnt;
//
//    printf("The amount of vertices is: %d\n", vertixAmnt);    //Point* a = &collect[0];
//    TransferPoint* TransferPoints = new TransferPoint[vertixAmnt];
//    for (int i = 0; i < vertixAmnt; ++i) {
//        TransferPoints[i].x = points[i].a;
//        TransferPoints[i].y = points[i].b;
//    }
//    //Get edges
//    std::vector<Point> collect;
//    for (voronoi_diagram<double>::const_vertex_iterator it =
//        vd.vertices().begin(); it != vd.vertices().end(); ++it) {
//        Point temp{ int(it->x()), int(it->y()) };
//        collect.push_back(temp);
//    }
//
//
//    printf("The length of the array is: %d\n", sizeof(TransferPoints) / sizeof(TransferPoints[0]));    //Point* a = &collect[0];
//    return TransferPoints;
//}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

