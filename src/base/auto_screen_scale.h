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
    double scaleRatio = 1.0;

    for (int i = 0; i < resources->noutput; i++)
    {
        XRROutputInfo *outputInfo = XRRGetOutputInfo(display, resources, resources->outputs[i]);
        if (outputInfo->crtc == 0 || outputInfo->mm_width == 0)
            continue;

        XRRCrtcInfo *crtInfo = XRRGetCrtcInfo(display, resources, outputInfo->crtc);
        if (crtInfo == nullptr)
            continue;

        scaleRatio = (double)crtInfo->width / (double)outputInfo->mm_width / (1366.0 / 310.0);

        if (scaleRatio > 1 + 2.0 / 3.0)
        {
            scaleRatio = 2;
        }
        else if (scaleRatio > 1 + 1.0 / 3.0)
        {
            scaleRatio = 1.5;
        }
        else
        {
            scaleRatio = 1;
        }
    }

    XRRFreeScreenResources(resources);

    qDebug() << "scale factor: " << QString::number(scaleRatio);

    setenv("QT_SCALE_FACTOR", const_cast<char *>(QString::number(1.25).toStdString().c_str()), 1);
}
} // namespace Utils
