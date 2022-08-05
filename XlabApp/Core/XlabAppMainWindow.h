#ifndef Project_MainWindow_H
#define Project_MainWindow_H

#include <QMainWindow>
#include < stack >
#include "ui_XlabAppMainWindow.h"
#include <vtkSmartPointer.h>

#include <QFileDialog>
#include <QDir>
#include <QStandardItemModel>
#include <QStandardItem>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkImageViewer2.h>
#include <QVTKWidget.h>
#include <vtkJPEGReader.h>
#include <vtkImageActor.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkCommand.h>
#include <vtkDICOMImageReader.h>
#include "vtkSmartPointer.h"
#include "vtkResliceImageViewer.h"
#include "vtkImagePlaneWidget.h"
#include "vtkDistanceWidget.h"
#include "vtkResliceImageViewerMeasurements.h"
#include "vtkResliceCursorLineRepresentation.h"
#include "vtkImageData.h"
#include "vtkCellPicker.h"

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include "vtkResliceImageViewer.h"
#include "vtkResliceCursorLineRepresentation.h"
#include "vtkResliceCursorThickLineRepresentation.h"
#include "vtkResliceCursorWidget.h"
#include "vtkResliceCursorActor.h"
#include "vtkResliceCursorPolyDataAlgorithm.h"
#include "vtkResliceCursor.h"
#include "vtkDICOMImageReader.h"
#include "vtkNIFTIImageReader.h"
#include "vtkCellPicker.h"
#include "vtkProperty.h"
#include "vtkPlane.h"
#include "vtkImageData.h"
#include "vtkCommand.h"
#include "vtkPlaneSource.h"
#include "vtkLookupTable.h"
#include "vtkImageMapToWindowLevelColors.h"
#include "vtkInteractorStyleImage.h"
#include "vtkImageSlabReslice.h"
#include "vtkBoundedPlanePointPlacer.h"
#include "vtkDistanceWidget.h"
#include "vtkDistanceRepresentation.h"
#include "vtkHandleRepresentation.h"
#include "vtkResliceImageViewerMeasurements.h"
#include "vtkDistanceRepresentation2D.h"
#include "vtkPointHandleRepresentation3D.h"
#include "vtkPointHandleRepresentation2D.h"

#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkNiftiImageIO.h"

class vtkImageViewer2;
class vtkRenderer;
class vtkEventQtSlotConnect;
class vtkObject;
class vtkCommand;
class vtkResliceImageViewer;
class vtkImagePlaneWidget;
class vtkDistanceWidget;
class vtkResliceImageViewerMeasurements;
class vtkResliceCursorCallback;

class XlabAppMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    XlabAppMainWindow(QWidget *parent = nullptr);
    ~XlabAppMainWindow();

private:
    Ui::XlabAppMainWindowClass *ui;

	int imageDims[3] = { 0 };
	int patientNIINum = 1;
	
	vtkSmartPointer < vtkResliceImageViewer > riw[3];
	vtkSmartPointer < vtkImagePlaneWidget > planeWidget[3];
	vtkSmartPointer < vtkDistanceWidget > DistanceWidget[3];
	vtkSmartPointer < vtkResliceImageViewerMeasurements > ResliceMeasurements;

	vtkSmartPointer < vtkDICOMImageReader > vtkDICOMReader = nullptr;
	vtkSmartPointer < vtkNIFTIImageReader > vtkNIIReader = nullptr;
	vtkSmartPointer < vtkCellPicker > picker;
	vtkSmartPointer < vtkProperty > ipwProp;
	vtkSmartPointer < vtkRenderer > ren;
	vtkRenderWindowInteractor* iren;
	vtkSmartPointer < vtkResliceCursorCallback > cbk;

    vtkSmartPointer < vtkImageViewer2 > m_pImageViewer;
    vtkSmartPointer < vtkRenderer > m_pRenderder;
    vtkEventQtSlotConnect* m_Connections;

	std::map<QString, QString> patientFileNameandPath;
	std::map<QString, int> patientDataMode;

	QStandardItemModel *model;
	
private slots:
	//��Ӧ��ͼ���ļ��Ĳۺ���
	void slotOpenDICOM();

	//��Ӧ��ʾ����ͼ
	void slotReadDICOMandNII();

	//��Ӧ��ͼ���ļ��Ĳۺ���
	void slotOpenNII();

	//��Ӧɾ������
	void slotDeleteDataList();

	//��Ӧ����ƶ�����Ϣ��ʵʱ������ĵ�ǰλ��
	void updateCoords(vtkObject* obj);

	//��Ӧ�رճ���
	void slotExitXlabApp();

	//��Ӧ��ʮ����
	void slotShowCross();
};
#endif
