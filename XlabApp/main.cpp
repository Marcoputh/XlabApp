#include "XlabAppMainWindow.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>

#include <vtkAutoInit.h> 

#include "vtkoutputwindow.h"

#pragma comment(lib,"ws2_32.lib") 
#pragma comment(lib,"Rpcrt4.lib")

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);

int main(int argc, char *argv[])
{
    vtkOutputWindow::SetGlobalWarningDisplay(0);
    QApplication a(argc, argv);
    XlabAppMainWindow w;
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    w.show();
    return a.exec();
}
