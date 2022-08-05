#include "XlabAppMainWindow.h"

class vtkResliceCursorCallback : public vtkCommand
{
public:
	static vtkResliceCursorCallback* New()
	{
		return new vtkResliceCursorCallback;
	}

	void Execute(vtkObject* caller, unsigned long ev,
		void* callData) override
	{

		if (ev == vtkResliceCursorWidget::WindowLevelEvent ||
			ev == vtkCommand::WindowLevelEvent ||
			ev == vtkResliceCursorWidget::ResliceThicknessChangedEvent)
		{
			// Render everything
			for (int i = 0; i < 3; i++)
			{
				this->RCW[i]->Render();
			}
			this->IPW[0]->GetInteractor()->GetRenderWindow()->Render();
			return;
		}

		vtkImagePlaneWidget* ipw =
			dynamic_cast<vtkImagePlaneWidget*>(caller);
		if (ipw)
		{
			double* wl = static_cast<double*>(callData);

			if (ipw == this->IPW[0])
			{
				this->IPW[1]->SetWindowLevel(wl[0], wl[1], 1);
				this->IPW[2]->SetWindowLevel(wl[0], wl[1], 1);
			}
			else if (ipw == this->IPW[1])
			{
				this->IPW[0]->SetWindowLevel(wl[0], wl[1], 1);
				this->IPW[2]->SetWindowLevel(wl[0], wl[1], 1);
			}
			else if (ipw == this->IPW[2])
			{
				this->IPW[0]->SetWindowLevel(wl[0], wl[1], 1);
				this->IPW[1]->SetWindowLevel(wl[0], wl[1], 1);
			}
		}

		vtkResliceCursorWidget* rcw = dynamic_cast<
			vtkResliceCursorWidget*>(caller);
		if (rcw)
		{
			vtkResliceCursorLineRepresentation* rep = dynamic_cast<
				vtkResliceCursorLineRepresentation*>(rcw->GetRepresentation());
			// Although the return value is not used, we keep the get calls
			// in case they had side-effects
			rep->GetResliceCursorActor()->GetCursorAlgorithm()->GetResliceCursor();
			for (int i = 0; i < 3; i++)
			{
				vtkPlaneSource* ps = static_cast<vtkPlaneSource*>(
					this->IPW[i]->GetPolyDataAlgorithm());
				ps->SetOrigin(this->RCW[i]->GetResliceCursorRepresentation()->
					GetPlaneSource()->GetOrigin());
				ps->SetPoint1(this->RCW[i]->GetResliceCursorRepresentation()->
					GetPlaneSource()->GetPoint1());
				ps->SetPoint2(this->RCW[i]->GetResliceCursorRepresentation()->
					GetPlaneSource()->GetPoint2());

				// If the reslice plane has modified, update it on the 3D widget
				this->IPW[i]->UpdatePlacement();
			}
		}

		// Render everything
		for (int i = 0; i < 3; i++)
		{
			this->RCW[i]->Render();
		}
		this->IPW[0]->GetInteractor()->GetRenderWindow()->Render();
	}

	vtkResliceCursorCallback() {}
	vtkImagePlaneWidget* IPW[3];
	vtkResliceCursorWidget* RCW[3];
};


XlabAppMainWindow::XlabAppMainWindow(QWidget *parent)
    : QMainWindow(parent),ui(new Ui::XlabAppMainWindowClass)
{
    ui->setupUi(this);

	model = new QStandardItemModel(ui->fileNameTree);
	model->setHorizontalHeaderLabels(QStringList() << QStringLiteral("患者姓名") << QStringLiteral("年龄") << QStringLiteral("性别"));
	ui->fileNameTree->setModel(model);

	// 初始化四视图，让他们都变成黑的
	for (int i = 0; i < 4; i++)
	{
		riw[i] = vtkSmartPointer< vtkResliceImageViewer >::New();
		vtkNew <vtkRenderWindow> renderWindow;
		riw[i]->SetRenderWindow(renderWindow);
	}

	ui->topleftvtk->SetRenderWindow(riw[0]->GetRenderWindow());
	riw[0]->SetupInteractor(
		ui->topleftvtk->GetRenderWindow()->GetInteractor());

	ui->toprightvtk->SetRenderWindow(riw[1]->GetRenderWindow());
	riw[1]->SetupInteractor(
		ui->toprightvtk->GetRenderWindow()->GetInteractor());

	ui->bottomleftvtk->SetRenderWindow(riw[2]->GetRenderWindow());
	riw[2]->SetupInteractor(
		ui->bottomleftvtk->GetRenderWindow()->GetInteractor());

	ui->bottomrightvtk->SetRenderWindow(riw[3]->GetRenderWindow());
	riw[3]->SetupInteractor(
		ui->bottomrightvtk->GetRenderWindow()->GetInteractor());
	

	////连接打开的信号与相应的槽
	connect(ui->openDICOM, SIGNAL(triggered()), this, SLOT(slotOpenDICOM()));
	connect(ui->openNII, SIGNAL(triggered()), this, SLOT(slotOpenNII()));
	connect(ui->viewButton, SIGNAL(clicked()), this, SLOT(slotReadDICOMandNII()));
	connect(ui->deleteDataButton, SIGNAL(clicked()), this, SLOT(slotDeleteDataList()));
	connect(ui->exitXlabApp, SIGNAL(triggered()), this, SLOT(slotExitXlabApp()));
	connect(ui->showCross, SIGNAL(triggered()), this, SLOT(slotShowCross()));
}

XlabAppMainWindow::~XlabAppMainWindow()
{
    delete ui;
}

void XlabAppMainWindow::slotOpenDICOM()
{
	char* patientName = new char[512];
	char* patienAge = new char[512];
	char* patienSex = new char[512];
	char unknown[8] = "Unkown";

	itk::GDCMImageIO::Pointer itkGdcmIO =
		itk::GDCMImageIO::New();
	itk::ImageSeriesReader< itk::Image< signed short, 3 > >::Pointer itkReader =
		itk::ImageSeriesReader< itk::Image< signed short, 3 > >::New();
	itk::GDCMSeriesFileNames::Pointer itkNamesGenerator =
		itk::GDCMSeriesFileNames::New();

    QDir dir;
	QString fileNameQstr = QFileDialog::getExistingDirectory(this, QString(tr("Open DICOM")), dir.absolutePath(), QFileDialog::ShowDirsOnly);
    //QString fileName = QFileDialog::getOpenFileName(this, QString(tr("打开图像")), dir.absolutePath(), filter);
    if (fileNameQstr.isEmpty() == true)
        return;

	itkNamesGenerator->SetInputDirectory(fileNameQstr.toLatin1().data());
	const itk::ImageSeriesReader< itk::Image< signed short, 3 > >::FileNamesContainer& filenames =
		itkNamesGenerator->GetInputFileNames();

	itkReader->SetImageIO(itkGdcmIO);
	itkReader->SetFileNames(filenames);

	itkReader->Update();
	itkReader->GetMetaDataDictionary();
	itkGdcmIO->GetMetaDataDictionary();
	itkGdcmIO->GetPatientName(patientName);
	itkGdcmIO->GetPatientAge(patienAge);
	itkGdcmIO->GetPatientSex(patienSex);


	if (patientName != nullptr && patientName[0] == '\0')
	{
		patientName = unknown;
	}

	QString patientNameQstr = QString(patientName);
	patientFileNameandPath.insert(std::pair<QString, QString>(patientNameQstr, fileNameQstr));
	patientDataMode.insert(std::pair<QString, int>(patientNameQstr, 0));

	QStandardItem* childItem = new QStandardItem(patientName);
	childItem->setCheckable(true);
	model->appendRow(childItem);

	if (patienAge != nullptr && patienAge[0] == '\0')
	{
		patienAge = unknown;
		model->setItem(model->indexFromItem(childItem).row(), 1, new QStandardItem(patienAge));
	}
	else
	{
		model->setItem(model->indexFromItem(childItem).row(), 1, new QStandardItem(patienAge));
	}

	if (patienSex != nullptr && patienSex[0] == '\0')
	{
		patienSex = unknown;
		model->setItem(model->indexFromItem(childItem).row(), 2, new QStandardItem(patienSex));
	}
	else
	{
		model->setItem(model->indexFromItem(childItem).row(), 2, new QStandardItem(patienSex));
	}
}

void XlabAppMainWindow::slotOpenNII()
{
	char* patientName = new char[512];
	char* patienAge = new char[512];
	char* patienSex = new char[512];
	char unknown[7] = "Unkown";
	char niiPatient[11] = "niiPatient";


	QString filter;
	filter = "NII image file (*.nii.gz)";

	QDir dir;
	QString fileName = QFileDialog::getOpenFileName(this, QString(tr("Open NII")), dir.absolutePath(), filter);
	if (fileName.isEmpty() == true)
		return;
	std::string filePath = fileName.toStdString();

	patientName = niiPatient;
	QString patientNameQstr = QString(patientName);
	patientNameQstr.append(QString::number(patientNIINum, 10));
	patientFileNameandPath.insert(std::pair<QString, QString>(patientNameQstr, fileName));
	patientDataMode.insert(std::pair<QString, int>(patientNameQstr, 1));

	QStandardItem* childItem = new QStandardItem(patientNameQstr.toLatin1().data());
	childItem->setCheckable(true);
	model->appendRow(childItem);
	patienAge = unknown;
	model->setItem(model->indexFromItem(childItem).row(), 1, new QStandardItem(patienAge));
	patienSex = unknown;
	model->setItem(model->indexFromItem(childItem).row(), 2, new QStandardItem(patienSex));
	patientNIINum++;
}

void XlabAppMainWindow::slotReadDICOMandNII()
{	
	ui->showCross->setEnabled(1);
	QModelIndex curIndex = ui->fileNameTree->currentIndex();
	int row = curIndex.row();
	int column = curIndex.column();

	//当前位置包含-1值返回
	if (-1 == row || -1 == column)
	{
		return;
	}

	QString patientNameQstr = curIndex.sibling(curIndex.row(), 0).data().toString();
	QString fileNameQstr = patientFileNameandPath[patientNameQstr];
	QByteArray ba = fileNameQstr.toLatin1();
	const char* fileNameStr = ba.data();
	int openMode = patientDataMode[patientNameQstr];

	//显示DICOM
	if (0 == openMode)
	{
		if (!vtkDICOMReader)
		{
			vtkDICOMReader = vtkSmartPointer < vtkDICOMImageReader >::New();
		}

		vtkDICOMReader->SetDirectoryName(fileNameStr);
		vtkDICOMReader->Update();
		vtkDICOMReader->GetOutput()->GetDimensions(imageDims);

		// 初始化三个视图
		for (int i = 0; i < 4; i++)
		{
			riw[i] = vtkSmartPointer< vtkResliceImageViewer >::New();
			vtkNew <vtkRenderWindow> renderWindow;
			riw[i]->SetRenderWindow(renderWindow);
		}

		ui->topleftvtk->SetRenderWindow(riw[0]->GetRenderWindow());
		riw[0]->SetupInteractor(
			ui->topleftvtk->GetRenderWindow()->GetInteractor());

		ui->toprightvtk->SetRenderWindow(riw[1]->GetRenderWindow());
		riw[1]->SetupInteractor(
			ui->toprightvtk->GetRenderWindow()->GetInteractor());

		ui->bottomleftvtk->SetRenderWindow(riw[2]->GetRenderWindow());
		riw[2]->SetupInteractor(
			ui->bottomleftvtk->GetRenderWindow()->GetInteractor());

		for (int i = 0; i < 3; i++)
		{
			// make them all share the same reslice cursor object.
			vtkResliceCursorLineRepresentation* rep =
				vtkResliceCursorLineRepresentation::SafeDownCast(
					riw[i]->GetResliceCursorWidget()->GetRepresentation());
			riw[i]->SetResliceCursor(riw[0]->GetResliceCursor());

			rep->GetResliceCursorActor()->
				GetCursorAlgorithm()->SetReslicePlaneNormal(i);

			riw[i]->SetInputData(vtkDICOMReader->GetOutput());
			riw[i]->SetSliceOrientation(i);
		}

		picker = vtkSmartPointer<vtkCellPicker>::New();
		picker->SetTolerance(0.005);

		ipwProp = vtkSmartPointer<vtkProperty>::New();
		ren = vtkSmartPointer< vtkRenderer >::New();

		vtkNew <vtkRenderWindow> renderWindow;
		ui->bottomrightvtk->SetRenderWindow(renderWindow);
		ui->bottomrightvtk->GetRenderWindow()->AddRenderer(ren);
		iren = ui->bottomrightvtk->GetInteractor();

		for (int i = 0; i < 3; i++)
		{
			planeWidget[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();
			planeWidget[i]->SetInteractor(iren);
			planeWidget[i]->SetPicker(picker);
			planeWidget[i]->RestrictPlaneToVolumeOn();
			double color[3] = { 0, 0, 0 };
			color[i] = 1;
			planeWidget[i]->GetPlaneProperty()->SetColor(color);

			color[0] /= 4.0;
			color[1] /= 4.0;
			color[2] /= 4.0;
			riw[i]->GetRenderer()->SetBackground(color);

			planeWidget[i]->SetTexturePlaneProperty(ipwProp);
			planeWidget[i]->TextureInterpolateOff();
			planeWidget[i]->SetResliceInterpolateToLinear();
			planeWidget[i]->SetInputConnection(vtkDICOMReader->GetOutputPort());
			planeWidget[i]->SetPlaneOrientation(i);
			planeWidget[i]->SetSliceIndex(imageDims[i] / 2);
			planeWidget[i]->DisplayTextOn();
			planeWidget[i]->SetDefaultRenderer(ren);
			planeWidget[i]->SetWindowLevel(1358, -27);
			planeWidget[i]->On();
			planeWidget[i]->InteractionOn();
		}

		cbk = vtkSmartPointer<vtkResliceCursorCallback>::New();
		// 合成三视图
		for (int i = 0; i < 3; i++)
		{
			cbk->IPW[i] = planeWidget[i];
			cbk->RCW[i] = riw[i]->GetResliceCursorWidget();
			riw[i]->GetResliceCursorWidget()->AddObserver(
				vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk);
			riw[i]->GetResliceCursorWidget()->AddObserver(
				vtkResliceCursorWidget::WindowLevelEvent, cbk);
			riw[i]->GetResliceCursorWidget()->AddObserver(
				vtkResliceCursorWidget::ResliceThicknessChangedEvent, cbk);
			riw[i]->GetResliceCursorWidget()->AddObserver(
				vtkResliceCursorWidget::ResetCursorEvent, cbk);
			riw[i]->GetInteractorStyle()->AddObserver(
				vtkCommand::WindowLevelEvent, cbk);

			// Make them all share the same color map.
			riw[i]->SetLookupTable(riw[0]->GetLookupTable());
			planeWidget[i]->GetColorMap()->SetLookupTable(riw[0]->GetLookupTable());
			//planeWidget[i]->GetColorMap()->SetInput(riw[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()->GetColorMap()->GetInput());
			planeWidget[i]->SetColorMap(riw[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()->GetColorMap());

		}
	}
	else if (1 == openMode) // 显示NII
	{
		if (!vtkNIIReader)
		{
			vtkNIIReader = vtkSmartPointer < vtkNIFTIImageReader >::New();
		}

		vtkNIIReader->SetFileName(fileNameStr);
		vtkNIIReader->Update();
		vtkNIIReader->GetOutput()->GetDimensions(imageDims);

		// 初始化三个视图
		for (int i = 0; i < 4; i++)
		{
			riw[i] = vtkSmartPointer< vtkResliceImageViewer >::New();
			vtkNew <vtkRenderWindow> renderWindow;
			riw[i]->SetRenderWindow(renderWindow);
		}

		ui->topleftvtk->SetRenderWindow(riw[0]->GetRenderWindow());
		riw[0]->SetupInteractor(
			ui->topleftvtk->GetRenderWindow()->GetInteractor());

		ui->toprightvtk->SetRenderWindow(riw[1]->GetRenderWindow());
		riw[1]->SetupInteractor(
			ui->toprightvtk->GetRenderWindow()->GetInteractor());

		ui->bottomleftvtk->SetRenderWindow(riw[2]->GetRenderWindow());
		riw[2]->SetupInteractor(
			ui->bottomleftvtk->GetRenderWindow()->GetInteractor());

		for (int i = 0; i < 3; i++)
		{
			// make them all share the same reslice cursor object.
			vtkResliceCursorLineRepresentation* rep =
				vtkResliceCursorLineRepresentation::SafeDownCast(
					riw[i]->GetResliceCursorWidget()->GetRepresentation());
			riw[i]->SetResliceCursor(riw[0]->GetResliceCursor());

			rep->GetResliceCursorActor()->
				GetCursorAlgorithm()->SetReslicePlaneNormal(i);

			riw[i]->SetInputData(vtkNIIReader->GetOutput());
			riw[i]->SetSliceOrientation(i);
		}

		picker = vtkSmartPointer<vtkCellPicker>::New();
		picker->SetTolerance(0.005);

		ipwProp = vtkSmartPointer<vtkProperty>::New();
		ren = vtkSmartPointer< vtkRenderer >::New();

		vtkNew <vtkRenderWindow> renderWindow;
		ui->bottomrightvtk->SetRenderWindow(renderWindow);
		ui->bottomrightvtk->GetRenderWindow()->AddRenderer(ren);
		iren = ui->bottomrightvtk->GetInteractor();

		for (int i = 0; i < 3; i++)
		{
			planeWidget[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();
			planeWidget[i]->SetInteractor(iren);
			planeWidget[i]->SetPicker(picker);
			planeWidget[i]->RestrictPlaneToVolumeOn();
			double color[3] = { 0, 0, 0 };
			color[i] = 1;
			planeWidget[i]->GetPlaneProperty()->SetColor(color);

			color[0] /= 4.0;
			color[1] /= 4.0;
			color[2] /= 4.0;
			riw[i]->GetRenderer()->SetBackground(color);

			planeWidget[i]->SetTexturePlaneProperty(ipwProp);
			planeWidget[i]->TextureInterpolateOff();
			planeWidget[i]->SetResliceInterpolateToLinear();
			planeWidget[i]->SetInputConnection(vtkNIIReader->GetOutputPort());
			planeWidget[i]->SetPlaneOrientation(i);
			planeWidget[i]->SetSliceIndex(imageDims[i] / 2);
			planeWidget[i]->DisplayTextOn();
			planeWidget[i]->SetDefaultRenderer(ren);
			planeWidget[i]->SetWindowLevel(1358, -27);
			planeWidget[i]->On();
			planeWidget[i]->InteractionOn();
		}

		cbk = vtkSmartPointer<vtkResliceCursorCallback>::New();
		// 合成三视图
		for (int i = 0; i < 3; i++)
		{
			cbk->IPW[i] = planeWidget[i];
			cbk->RCW[i] = riw[i]->GetResliceCursorWidget();
			riw[i]->GetResliceCursorWidget()->AddObserver(
				vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk);
			riw[i]->GetResliceCursorWidget()->AddObserver(
				vtkResliceCursorWidget::WindowLevelEvent, cbk);
			riw[i]->GetResliceCursorWidget()->AddObserver(
				vtkResliceCursorWidget::ResliceThicknessChangedEvent, cbk);
			riw[i]->GetResliceCursorWidget()->AddObserver(
				vtkResliceCursorWidget::ResetCursorEvent, cbk);
			riw[i]->GetInteractorStyle()->AddObserver(
				vtkCommand::WindowLevelEvent, cbk);

			// Make them all share the same color map.
			riw[i]->SetLookupTable(riw[0]->GetLookupTable());
			planeWidget[i]->GetColorMap()->SetLookupTable(riw[0]->GetLookupTable());
			//planeWidget[i]->GetColorMap()->SetInput(riw[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()->GetColorMap()->GetInput());
			planeWidget[i]->SetColorMap(riw[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()->GetColorMap());

		}
	}
}

void XlabAppMainWindow::slotDeleteDataList()
{
	std::stack < int > deleteRowIndex;
	int childNum = model->rowCount();
	for (int i = 0; i < childNum; i++)
	{
		QStandardItem* childItem = model->item(i);
		if (childItem->checkState())
		{
			deleteRowIndex.push(i);
		}
	}
	while (!deleteRowIndex.empty())
	{
		model->removeRow(deleteRowIndex.top());
		deleteRowIndex.pop();
	}
}

void XlabAppMainWindow::updateCoords(vtkObject* obj)
{
    // 获取交互器
    vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(obj);

    // 获取鼠标的当前位置
    int event_pos[2];
    iren->GetEventPosition(event_pos);

    QString str;
    str.sprintf("x=%d : y=%d", event_pos[0], event_pos[1]);
    //m_StatusBar->showMessage(str);
}

void XlabAppMainWindow::slotExitXlabApp()
{
	QApplication::quit();
}

void XlabAppMainWindow::slotShowCross()
{
	if (riw[0]->GetResliceMode())
	{
		for (int i = 0; i < 3; i++)
		{
			riw[i]->SetResliceMode(0);
			riw[i]->GetRenderer()->ResetCamera();
			riw[i]->Render();
		}
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			riw[i]->SetResliceMode(1);
			riw[i]->GetRenderer()->ResetCamera();
			riw[i]->Render();
		}
	}
}
