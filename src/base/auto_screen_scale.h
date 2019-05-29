#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

#include <QString>
#include <QDebug>

namespace Utils
{
void AutoScreenScale()
{
    double scaleRatio = 1.0;
    Display *display = XOpenDisplay(NULL);

    XRRScreenResources *resources = XRRGetScreenResourcesCurrent(display, DefaultRootWindow(display));

    if (!resources) {
        resources = XRRGetScreenResources(display, DefaultRootWindow(display));
        qWarning() << "get XRRGetScreenResourcesCurrent failed, use XRRGetScreenResources.";
    }

    if (resources) {
        std::list<double> scaleRatioList;
        for (int i = 0; i < resources->noutput; i++)
        {
            XRROutputInfo *outputInfo = XRRGetOutputInfo(display, resources, resources->outputs[i]);
            if (outputInfo == nullptr) {
                continue;
            }

            if (outputInfo->crtc == 0 || outputInfo->mm_width == 0)
                continue;

            XRRCrtcInfo *crtInfo = XRRGetCrtcInfo(display, resources, outputInfo->crtc);
            if (crtInfo == nullptr) {
                continue;
            }

            scaleRatio = static_cast<double>(crtInfo->width) / static_cast<double>(outputInfo->mm_width) / (1366.0 / 310.0);

            // When you have multiple screens,
            // if one of the resolutions is very large,
            // it will cause the copy mode to display abnormally.
            if (scaleRatio > 1 + 2.0 / 3.0) {
                scaleRatioList.push_back(2.0);
            }
            else if (scaleRatio > 1 + 1.0 / 3.0) {
                scaleRatioList.push_back(1.5);
            }
            else {
                scaleRatioList.push_back(1.0);
            }
        }

        if (!scaleRatioList.empty()) {
            scaleRatioList.sort();
            scaleRatio = *scaleRatioList.begin();
        }

        XRRFreeScreenResources(resources);
    }

    qDebug() << "scale factor: " << QString::number(scaleRatio);
    qputenv("QT_SCALE_FACTOR", QString::number(scaleRatio).toUtf8());
}
} // namespace Utils
