#include "file.h"

using namespace G;

File::File() {}

File::~File()
{
    qDeleteAll(apertures);
    if (itemGroup)
        delete itemGroup;
}

Paths File::Merge()
{
    Paths paths;
    Paths tmpPaths;
    //    file = gerberFile;
    int i = 0, exp = -1;
    Clipper clipper(ioStrictlySimple);
    while (i < size()) {
        clipper.Clear();
        clipper.AddPaths(paths, ptSubject, true);
        exp = at(i).state.imgPolarity;
        do {
            tmpPaths = at(i++).paths;
            SimplifyPolygons(tmpPaths, pftNonZero);
            clipper.AddPaths(tmpPaths, ptClip, true);
        } while (i < size() && exp == at(i).state.imgPolarity);
        if (at(i - 1).state.imgPolarity == G::POSITIVE) {
            clipper.Execute(ctUnion, paths, pftPositive);
        } else {
            clipper.Execute(ctDifference, paths, pftNonZero);
        }
    }
    mergedPaths = paths;
    return mergedPaths;
}

Pathss& File::GetGroupedPaths(GROUP group, bool fl)
{
    PolyTree polyTree;
    Clipper clipper;
    clipper.AddPaths(mergedPaths, ptSubject, true);
    IntRect r(clipper.GetBounds());
    int k = /*uScale*/ 1;
    Path outer = {
        IntPoint(r.left - k, r.bottom + k),
        IntPoint(r.right + k, r.bottom + k),
        IntPoint(r.right + k, r.top - k),
        IntPoint(r.left - k, r.top - k)
    };
    if (fl)
        ReversePath(outer);
    clipper.AddPath(outer, ptSubject, true);
    clipper.Execute(ctUnion, polyTree, pftNonZero);
    grouping(polyTree.GetFirst(), &groupedPaths, group);
    return groupedPaths;
}

void File::grouping(PolyNode* node, Pathss* pathss, File::GROUP group)
{
    Path path;
    Paths paths;
    switch (group) {
    case CUTOFF:
        if (!node->IsHole()) {
            path = node->Contour;
            paths.push_back(path);
            for (int var = 0; var < node->ChildCount(); ++var) {
                path = node->Childs[var]->Contour;
                paths.push_back(path);
            }
            pathss->push_back(paths);
        }
        for (int var = 0; var < node->ChildCount(); ++var) {
            grouping(node->Childs[var], pathss, group);
        }
        break;
    case COPPER:
        if (node->IsHole()) {
            path = node->Contour;
            paths.push_back(path);
            for (int var = 0; var < node->ChildCount(); ++var) {
                path = node->Childs[var]->Contour;
                paths.push_back(path);
            }
            pathss->push_back(paths);
        }
        for (int var = 0; var < node->ChildCount(); ++var) {
            grouping(node->Childs[var], pathss, group);
        }
        break;
    }
}