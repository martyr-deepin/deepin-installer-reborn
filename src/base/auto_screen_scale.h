#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

#include <QString>
#include <QDebug>

namespace Utils
{
void AutoScreenScale()
{
    Display *display = XOpenDisplay(NULL);

    XRRScreenResources *resources = XRRGetScreenResources(display, DefaultRootWindow(display));
    // NOTE(lxz): Screens that require 4x scale may not be encountered
    double scaleRatio = 4.0;

    for (int i = 0; i < resources->noutput; i++)
    {
        XRROutputInfo *outputInfo = XRRGetOutputInfo(display, resources, resources->outputs[i]);
        if (outputInfo->crtc == 0 || outputInfo->mm_width == 0)
            continue;

        XRRCrtcInfo *crtInfo = XRRGetCrtcInfo(display, resources, outputInfo->crtc);
        if (crtInfo == nullptr)
            continue;

        scaleRatio = static_cast<double>(crtInfo->width) / static_cast<double>(outputInfo->mm_width) / (1366.0 / 310.0);

        // When you have multiple screens,
        // if one of the resolutions is very large,
        // it will cause the copy mode to display abnormally.
        if (scaleRatio > 1 + 2.0 / 3.0) {
            scaleRatio = std::min(2.0, scaleRatio);
        }
        else if (scaleRatio > 1 + 1.0 / 3.0) {
            scaleRatio = std::min(1.5, scaleRatio);
        }
        else {
            scaleRatio = 1.0;
        }
    }

    XRRFreeScreenResources(resources);

    qDebug() << "scale factor: " << QString::number(scaleRatio);

    setenv("QT_SCALE_FACTOR", const_cast<char *>(QString::number(scaleRatio).toStdString().c_str()), 1);
}
} // namespace Utils
