#include "StdAfx.h"
#include "GridTableCompareView.h"
#include "XLComparator.h"
#include "ExcelDataExtraction.h"
#include "CommonHeader.h"
#include "Settings.h"
#include "GridViewHelper.h"
#include "HistoryManager.h"

GridTableCompareView* GridTableCompareView::m_Instance = 0;;

#define FIRST_TABLE 0
#define SECOND_TABLE 1


enum DIFFIndicatorStates
{
	States_Begin,
	Both_NotVisible,
	FirstVisible_SecondNotVisible,
	FirstNotVisible_SecondVisible,
	Both_Visible,

	Change_Value,
	Value_Changed,

	Check_Visibility,

	Check_States_Feasibility,

	EndState,

	MaxIndicatorStates
};

GridTableCompareView* GridTableCompareView::GetInstance()
{
	if(!m_Instance)
	{
		m_Instance = new GridTableCompareView();
	}
	return m_Instance;
}

GridTableCompareView::GridTableCompareView():
m_pathOne(" "),
m_pathTwo(" "),
m_ContainerList(3, 0),
m_iX(FIRST_TABLE_X),
m_iY(FIRST_TABLE_Y),
m_iTableWidth(TABLE_WIDTH),
m_iTableHeight(TABLE_HEIGHT),
m_iRowHeight(CELL_HEIGHT),
m_GridList(3, 0),
m_bReady(false),
m_iMaxRow(10),
m_iMaxCol(10),
m_pParentWnd(0),
m_CompItr(&m_iChangedCols)
{
	m_CurrentState = XLTableParamFullTable;
}

GridTableCompareView::~GridTableCompareView(void)
{
	ResetTables();
	ResetContainers();
}

void GridTableCompareView::Clear()
{
	ResetTables();
	ResetContainers();
}


bool GridTableCompareView::Load()
{
	ClearHistory();
	bool fileA = LoadTableA(m_pathOne);
	bool fileB = LoadTableB(m_pathTwo);
	return (fileA & fileB);
}

void GridTableCompareView::m_SetMaxRowColForGridCreation()
{
	m_iMaxRow = 10;
	m_iMaxCol = 10;
	if(m_ContainerList[FIRST_TABLE] &&	m_ContainerList[SECOND_TABLE])
	{
		if(m_ContainerList[FIRST_TABLE]->Row() > m_ContainerList[SECOND_TABLE]->Row())
		{
			m_iMaxRow =	 m_ContainerList[FIRST_TABLE]->Row();
		}
		else
		{
			m_iMaxRow =	 m_ContainerList[SECOND_TABLE]->Row();
		}
		if(m_ContainerList[FIRST_TABLE]->Col() > m_ContainerList[SECOND_TABLE]->Col())
		{
			m_iMaxCol =	 m_ContainerList[FIRST_TABLE]->Col();
		}
		else
		{
			m_iMaxCol =	m_ContainerList[SECOND_TABLE]->Col();
		}
	}
	else if(m_ContainerList[FIRST_TABLE])
	{
		m_iMaxRow =	 m_ContainerList[FIRST_TABLE]->Row();
		m_iMaxCol =	 m_ContainerList[FIRST_TABLE]->Col();
	}
	else if(m_ContainerList[SECOND_TABLE])
	{
		m_iMaxRow =	 m_ContainerList[SECOND_TABLE]->Row();
		m_iMaxCol =	 m_ContainerList[SECOND_TABLE]->Col();
	}
}

bool GridTableCompareView::LoadTableA(std::string path)
{
	bool bRet = false;
	try
	{
		m_pathOne = path;
		CExcelDataExtraction e;
		if(!m_ContainerList[FIRST_TABLE])
		{
			delete m_ContainerList[FIRST_TABLE];
			m_ContainerList[FIRST_TABLE] = 0;
		}
		m_ContainerList[FIRST_TABLE] = e.GetXLContainerExcelFormat(m_pathOne);
		if(!m_ContainerList[FIRST_TABLE])
		{
			return bRet;
		}
		if(!m_ContainerList[FIRST_TABLE] || !m_ContainerList[SECOND_TABLE])
		{
			m_bReady = false;
		}
		else
		{
			m_bReady = true;
		}
		m_SetMaxRowColForGridCreation();
		CreateGrid(m_GridList[FIRST_TABLE], m_ContainerList[FIRST_TABLE], FIRST_TABLE);
		EnableHistory(false);
		for(int j = 0; j < m_ContainerList[FIRST_TABLE]->Row(); j++)
		{
			for(int k = 0; k < m_ContainerList[FIRST_TABLE]->Col(); k++)
			{
				m_fillGrid(m_GridList[FIRST_TABLE], m_ContainerList[FIRST_TABLE], j, k);
			}
		}
		EnableHistory(true);
		bRet = true;
	}
	catch(...)
	{
	}
	return bRet;
}


bool GridTableCompareView::LoadTableB(std::string path)
{
	bool bRet = false;
	try
	{
		m_pathTwo = path;
		CExcelDataExtraction e;
		if(!m_ContainerList[SECOND_TABLE])
		{
			delete m_ContainerList[SECOND_TABLE];
			m_ContainerList[SECOND_TABLE] = 0;
		}
		m_ContainerList[SECOND_TABLE] = e.GetXLContainerExcelFormat(m_pathTwo);
		if(!m_ContainerList[SECOND_TABLE])
		{
			return bRet;
		}
		if(!m_ContainerList[FIRST_TABLE] || !m_ContainerList[SECOND_TABLE])
		{
			m_bReady = false;
		}
		else
		{
			m_bReady = true;
		}
		m_SetMaxRowColForGridCreation();
		CreateGrid(m_GridList[SECOND_TABLE], m_ContainerList[SECOND_TABLE], SECOND_TABLE);
		EnableHistory(false);
		for(int j = 0; j < m_ContainerList[SECOND_TABLE]->Row(); j++)
		{
			for(int k = 0; k < m_ContainerList[SECOND_TABLE]->Col(); k++)
			{
				m_fillGrid(m_GridList[SECOND_TABLE], m_ContainerList[SECOND_TABLE], j, k);
			}
		}
		EnableHistory(true);
		bRet = true;
	}
	catch(...)
	{
	}
	return bRet;
}


void GridTableCompareView::CreateTable()
{
	try
	{
		if(IsReady() && m_pParentWnd)
		{
			int numTables = NumberofDrawableTables();
			XL_UTIL->VerticallyDivideArea(numTables, m_Partitions, m_iTableWidth);
			for(int i = 0; i < m_ContainerList.size(); i++)
			{
				if(!m_ContainerList[i])
					continue;
				CreateGrid(m_GridList[i], m_ContainerList[i], i);
				EnableHistory(false);
				for(int j = 0; j < m_ContainerList[i]->Row(); j++)
				{
					for(int k = 0; k < m_ContainerList[i]->Col(); k++)
					{
						m_fillGrid(m_GridList[i], m_ContainerList[i], j, k);
					}
				}
				EnableHistory(true);
			}
		}
	}
	catch(...)
	{
	}
}
void GridTableCompareView::FindBackward(std::wstring& wstr)
{
	int i = m_NextFind.X();
	int j = m_NextFind.Y() - 1;
	CellIdentifier gCell;

	CGridCtrl* grid = 0;
	;
	XLCellDataContainer* container = 0;
	int x = 0;

	if(GetCurrentObserver() == m_GridList[0])
	{
		grid = m_GridList[0];
		container = m_ContainerList[0];
		x = 0;
	}
	else
	{
		grid = m_GridList[1];
		container = m_ContainerList[1];
		x = 1;
	}

	if(container)
	{
		while(i >= 0)
		{
			while(j >= 0)
			{
				XLCellData* data = container->CellData(i, j);
				if(data)
				{
					if(data->ForceCompare(wstr))
					{
						m_NextFind.Set(i, j);
						if(!m_GridToContainerMap[x].FindSecond(m_NextFind, gCell))
						{
							continue;
						}
						grid->GoToCell(i, j);
						return;
					}
				}
				j--;
			}
			j = container->Col() - 1;
			i--;
		}
	}
}


void GridTableCompareView::FindForward(std::wstring& wstr)
{
	int i = m_NextFind.X();
	int j = m_NextFind.Y() + 1;
	if( i < 0)
	{
		i = 0;
	}
	if(j < 0)
	{
		 j = 0;
	}
	CellIdentifier gCell;

	CGridCtrl* grid = 0;
	;
	XLCellDataContainer* container = 0;
	int x = 0;

	if(GetCurrentObserver() == m_GridList[0])
	{
		grid = m_GridList[0];
		container = m_ContainerList[0];
		x = 0;
	}
	else
	{
		grid = m_GridList[1];
		container = m_ContainerList[1];
		x = 1;
	}

	if(container)
	{
		while(i < container->Row())
		{
			while(j < container->Col())
			{
				XLCellData* data = container->CellData(i, j);
				if(data)
				{
					if(data->ForceCompare(wstr))
					{
						m_NextFind.Set(i, j);
						if(!m_GridToContainerMap[x].FindSecond(m_NextFind, gCell))
						{
							continue;
						}
						grid->GoToCell(i, j);
						return;
					}
				}
				j++;
			}
			j = 0;
			i++;
		}
	}
}


void GridTableCompareView::Find(std::wstring& wstr)
{
	int table = 0;
	CGridCtrl* grid = 0;
	if(GetCurrentObserver() == m_GridList[0])
	{
		grid = m_GridList[0];
		table = 0;
	}
	else
	{
		grid = m_GridList[1];
		table = 1;
	}
	if(grid)
	{
		CCellID id = grid->GetMouseSelectedCell();
		CellIdentifier gCell(id.row, id.col);
		if(m_GridToContainerMap[table].FindSecond(gCell, m_NextFind))
		{	
			CSettingsSM::GetInstance()->InitUserSearchStringComparator();
			if(CSettingsSM::GetInstance()->GetDefaultSearchDirection() == DefaultSearchDir_DOWN)
			{
				FindForward(wstr);
			}
			else
			{
				FindBackward(wstr);
			}
		}
	}
}


void GridTableCompareView::CreateGrid(CGridCtrl*& grid, XLCellDataContainer* container, int position)
{
	try
	{
		if(!container || !m_pParentWnd)
		{
			return;

		}
		if(grid)
		{
			//CCellRange Selection(0,0, container->Row() - 1, container->Col() - 1);
			//grid->ClearCells(Selection);
			delete grid;
			grid = 0;
		}
		//else
		{ 	
			CRect rect;
			int numPartitions = NumberofDrawableTables();

			int r = m_iMaxRow;
			int c = m_iMaxCol;
			if(r < container->Row())
			{
				r = container->Row();
			}
			if(c < container->Col())
			{
				c = container->Col();
			}

			int winHeight = XL_UTIL->GetWindowHeight();

			int h = (r*m_iRowHeight)> winHeight? (winHeight - 160): (r*m_iRowHeight);

			rect.bottom = (m_iY + h);
			rect.left = m_Partitions[position];
			rect.right = rect.left + m_iTableWidth;
			rect.top = m_iY;
			grid = new CGridCtrl(this);
			grid->Create(rect, m_pParentWnd, position);
			grid->SetRowCount(r + 2);
			grid->EnableDragAndDrop(FALSE);
			grid->SetColumnCount(c);
			grid->SetEditable(TRUE);
			grid->SetAutoSizeStyle();
			//grid->EnableDragAndDrop();
			grid->SetGridLines();
			grid->SetColumnResize(TRUE);
			//grid->AutoSizeColumns();
		}
	}
	catch(...)
	{
	}
}

void GridTableCompareView::CreateEmptyGrid()
{
	try
	{
		int numPartitions = NumberofDrawableTables();
		XL_UTIL->VerticallyDivideArea(2, m_Partitions, m_iTableWidth);
		int winHeight = XL_UTIL->GetWindowHeight();
		for(int i = 0; i < 2; i++)
		{ 	
			if(m_GridList[i])
			{
				delete m_GridList[i];
				m_GridList[i] = 0;
			}
			CRect rect;		

			int r = 400;
			int c = 60;
			int h = (r*m_iRowHeight)> winHeight? (winHeight - 160): (r*m_iRowHeight);

			rect.bottom = (m_iY + h);
			rect.left = m_Partitions[i];
			rect.right = rect.left + m_iTableWidth;
			rect.top = m_iY;
			m_GridList[i] = new CGridCtrl(this);
			m_GridList[i]->Create(rect, m_pParentWnd, i);
			m_GridList[i]->SetRowCount(r + 2);
			m_GridList[i]->EnableDragAndDrop(FALSE);
			m_GridList[i]->SetColumnCount(0);
			//m_GridList[i]->SetGridBkColor(WHITECOL);
			m_GridList[i]->SetTextColor(BLUECOL);
			//m_GridList[i]->SetEditable(TRUE);
			m_GridList[i]->SetAutoSizeStyle();
			//m_GridList[i]->EnableDragAndDrop();
			//m_GridList[i]->SetGridLines();
		}
	}
	catch(...)
	{
	}
}

void GridTableCompareView::ArrowPosition(int X, int Y)
{
	 if(X < m_Partitions[FIRST_TABLE])
	 {
		 int Row = m_GridList[FIRST_TABLE]->GetRow(X + FIRST_TABLE_X, Y) - 1;
		 CellIdentifier gridCell(Row, 1);
		 CellIdentifier contCell;
		 m_GridToContainerMap[FIRST_TABLE].FindSecond(gridCell, contCell);
		 
		 RowComparison* rc = m_iChangedCols.FindRow1(contCell.X());
		 if(rc)
		 {
			 m_CompItr.MoveToIndex(m_iChangedCols.FindIndex1(contCell.X()));
		 }
		 MoveIndicators();
	 }
	 else if( X > (m_Partitions[SECOND_TABLE] + m_iTableWidth))
	 {
		 int Row = m_GridList[SECOND_TABLE]->GetRow(X - LEFT_GAP, Y) - 1;
		 CellIdentifier gridCell(Row, 1);
		 CellIdentifier contCell;
		 m_GridToContainerMap[SECOND_TABLE].FindSecond(gridCell, contCell);
		 
		 RowComparison* rc = m_iChangedCols.FindRow2(contCell.X());
		 if(rc)
		 {
			 m_CompItr.MoveToIndex(m_iChangedCols.FindIndex2(contCell.X()));
		 }
		 MoveIndicators();
	 }
	 //Arrow move
 }


void GridTableCompareView::m_fillGrid(CGridCtrl* grid, XLCellDataContainer* container, int r, int c, int gr, int gc)
{
	if(!grid || !container)
	{
		return;
	}
	if(r < 0 || c < 0 || gr < 0 || gc < 0)
	{
		return;
	}
	if((r >= container->Row()) || (c >= container->Col()))
	{
		return;
	}

	GV_ITEM item;
	item.row = r;
	item.col = c;
	XLCellData* data = container->CellData(r, c);
	
	switch(data->Type())
	{
	case Integer:
		{
			char buffer[1024];
			int dataValue = 0;
			data->GetData(dataValue);
			itoa(dataValue, buffer, 10);
			item.strText = buffer;
		}
		break;
	case Double:
		{
			double dataValue = 0.0;
			data->GetData(dataValue);
			std::ostringstream sstream;
			sstream << dataValue;
			std::string varAsString = sstream.str();
			item.strText = varAsString.c_str();
		}
		break;
	case Char:
		{
			char dataValue = 0;
			data->GetData(dataValue);
			item.strText = dataValue;
		}
		break;
	case String:
		{
			std::string dataValue;
			data->GetData(dataValue);
			item.strText = dataValue.c_str();
		}
		break;
	case WString:
		{
			std::wstring dataValue;
			data->GetData(dataValue);
			item.strText = dataValue.c_str();
		}
		break;
	default:
		break;
	}
	bool okay;
	okay = grid->SetItemText(gr, gc, item.strText);

	CellIdentifier gridIdentifier(gr, gc);
	CellIdentifier containerIdentifier(r, c); 	

	if(grid == m_GridList[0])
	{
		m_GridToContainerMap[0].Insert(gridIdentifier, containerIdentifier);
	}
	else
	{
		m_GridToContainerMap[1].Insert(gridIdentifier, containerIdentifier);
	}
	
	
}

void GridTableCompareView::RecordInHistory(int tableID, CellIdentifier contCell, XLCellData* data)
{
	HistDataPair histData;
	histData.Data = *data;
	histData.TableID = tableID;
	histData.Cell = contCell; 
	InsertHistory(histData);
}


void GridTableCompareView::m_fillGrid(CGridCtrl* grid, XLCellDataContainer* container, int r, int gr, int c)
{
	m_fillGrid(grid, container, r, c, gr, c);
}


void GridTableCompareView::m_fillGrid(CGridCtrl* grid, XLCellDataContainer* container, int r, int c)
{
	m_fillGrid(grid, container, r, c, r, c);
}


CompareResult GridTableCompareView::Compare(XLTableParam param)
{
	CompareResult result = CompareResult_Unsuccessful;
	m_CurrentState = param;
	m_NextFind.Reset();
	m_GridToContainerMap[0].Clear();
	m_GridToContainerMap[1].Clear();
	NotifyFileSaved();
	//ClearHistory();
	if(m_ContainerList[FIRST_TABLE] && m_ContainerList[SECOND_TABLE])
	{
		m_ContainerList[FIRST_TABLE]->ResizeToNew(m_iMaxRow, m_iMaxCol);
		m_ContainerList[SECOND_TABLE]->ResizeToNew(m_iMaxRow, m_iMaxCol);

		if(m_Comparator.Compare(m_ContainerList[FIRST_TABLE], m_ContainerList[SECOND_TABLE]))
		{
			if(m_Comparator.ChangedRows().size() == 0)
			{
				result = CompareResult_Success_NoChange;
			}
			else
			{
				result = CompareResult_Success_Changes;
			}
			CreateComparisonDataStructure();
			switch(param)
			{
			case XLTableParamFullTable:
				CreateTable();
				FormatCells();
				MoveIndicators();
				break;
			case XLTableParamDiffTable:
				mDrawDiffTable();
				MoveIndicators();
				break;
			case XLTableParamSameTable:
				mDrawSameTable();
				MoveIndicators(true);
				break;
			default:
				break;
			};
			Notify(0, GRID_ON_COMPARE_DONE);
			
		}
		
	}
	return result;
}

void GridTableCompareView::GoToRow(int row, int table)
{
	if(table < 0 || table > 2)
	{
		return;
	}
	if(!m_GridList[table])
	{
		return;
	}
	m_GridList[table]->GoToCell(row, 0);
}
void GridTableCompareView::CreateComparisonDataStructure()
{

	m_iChangedCols.clear();
	if(m_Comparator.ChangedRows().size() > 0)
	{
		m_iChangedCols.resize(m_Comparator.ChangedRows().size());
	}

	for(int i = 0; i < m_Comparator.ChangedRows().size(); i++)
	{
		int j = 0;
		int k = 0;
		int row1 = m_Comparator.ChangedRows()[i].first;
		int row2 = m_Comparator.ChangedRows()[i].second;

		RowComparison* rc = m_iChangedCols.Insert(row1, row2);
		if(!rc)
		{
			continue;
		}

		while( (j < m_ContainerList[0]->Col()) && (k < m_ContainerList[1]->Col()))
		{
			XLCellData* one = m_ContainerList[0]->CellData(row1,j);
			XLCellData* two = m_ContainerList[1]->CellData(row2,j);
			if(!one || !two)
			{
				break;
			}
			if(*one == *two)
			{
			}
			else
			{
				rc->Insert(j, k);
			}
			j++;
			k++;
		}
		while(j < m_ContainerList[0]->Col())
		{
			rc->Insert(j, k);
			j++;
			k++;
		}
		while(k < m_ContainerList[1]->Col())
		{
			rc->Insert(j, k);
			k++;
			j++;
		}
		
	}
	m_CompItr.Reset();
	
}

void GridTableCompareView::SetRowColor(int table, int r, COLORREF ref)
{
	m_GridList[table]->SetItemBkColour(r + 1,  0, ref);
}

void GridTableCompareView::NextDifference()
{
	if(!m_GridList[FIRST_TABLE] || !m_GridList[SECOND_TABLE] || (m_Comparator.ChangedRows().size() == 0))
	{
		return;
	}
	

	CellIdentifier g1, c1, g2, c2;

	int first = 0;//m_Comparator.ChangedRows()[m_CurrentDiffItr.Itr].first;
	int second = 0;//m_Comparator.ChangedRows()[m_CurrentDiffItr.Itr].second;
	bool FirstVisible = false;
	bool SecondVisible = false;
	bool FeasibilityCheck = false;

	DIFFIndicatorStates states = States_Begin;
	DIFFIndicatorStates prevState = states;
	while(states != MaxIndicatorStates)
	{
		switch(states)
		{
		case States_Begin:
			if(!FeasibilityCheck)
			{
				prevState = states;
				states = Check_States_Feasibility;
			}
			else
			{	
				FeasibilityCheck = false;
				states = Check_Visibility;
			}
			break;
		case Both_NotVisible:
			states = Change_Value;
			
			break;
		case FirstVisible_SecondNotVisible:
			if(first > second)
			{
				m_GridList[SECOND_TABLE]->GoToCell(second, 0);
				states = EndState;
			}
			else
			{
				states = Change_Value;
			}
			break;
		case FirstNotVisible_SecondVisible:
			if(first < second)
			{
				m_GridList[FIRST_TABLE]->GoToCell(first, 0);
				states = EndState;
			}
			else
			{
				states = Change_Value;
			}
			break;
		case Both_Visible:
			states = Change_Value;
			break;
		case Check_States_Feasibility:
			if(!m_CompItr.GetValue())
			{
				m_CompItr.Reset();
				states = EndState;
			}
			else
			{
				states = prevState;
			}
			FeasibilityCheck = true;
			break;
		case Change_Value:
			if(m_CompItr.Next())
			{
				m_CompItr++;
				states = Value_Changed;
			}
			else
			{
				states = EndState;
			}
			break;
		case Value_Changed:
			if(!FeasibilityCheck)
			{
				prevState = states;
				states = Check_States_Feasibility;
			}
			else
			{
				FeasibilityCheck = false;
				int r1 = m_CompItr.GetValue()->Row1;
				int r2 = m_CompItr.GetValue()->Row2;
				c1.Set(r1, 0);
				if(m_GridToContainerMap[0].FindFirst(c1, g1))
				{
					first = g1.X();
				}
				c2.Set(r2, 0);
				if(m_GridToContainerMap[1].FindFirst(c2, g2))
				{
					second = g2.X();
				}				
				if(first > second)
				{
					m_GridList[FIRST_TABLE]->GoToCell(first, 0);
				}
				else
				{
					m_GridList[SECOND_TABLE]->GoToCell(second, 0);
				}
				states = EndState;
			}
			break;
		case Check_Visibility:
			{
				int r1 = m_CompItr.GetValue()->Row1;
				int r2 = m_CompItr.GetValue()->Row2;
				c1.Set(r1, 0);
				if(m_GridToContainerMap[0].FindFirst(c1, g1))
				{
					first = g1.X();
				}
				c2.Set(r2, 0);
				if(m_GridToContainerMap[1].FindFirst(c2, g2))
				{
					second = g2.X();
				}

				FirstVisible = m_GridList[FIRST_TABLE]->IsRowVisible(first);
				SecondVisible = m_GridList[SECOND_TABLE]->IsRowVisible(second);
				if(FirstVisible &&   SecondVisible)
				{
					states = Both_Visible;
				}
				else if(FirstVisible)
				{
					states = FirstVisible_SecondNotVisible;
				}
				else if(SecondVisible)
				{
					states = FirstNotVisible_SecondVisible;
				}
				else
				{
					states = Both_NotVisible;
				}
			}
			break;
		case EndState:
			FeasibilityCheck = false;
			MoveIndicators();
			states = MaxIndicatorStates;
			break;
		default:
			states = MaxIndicatorStates;
			break;
		}
	}

	
}

void GridTableCompareView::PrevDifference()
{
	if(!m_GridList[FIRST_TABLE] || !m_GridList[SECOND_TABLE] || (m_Comparator.ChangedRows().size() == 0))
	{
		return;
	}
	

	CellIdentifier g1, c1, g2, c2;

	int first = 0;//m_Comparator.ChangedRows()[m_CurrentDiffItr.Itr].first;
	int second = 0;//m_Comparator.ChangedRows()[m_CurrentDiffItr.Itr].second;
	bool FirstVisible = false;
	bool SecondVisible = false;
	bool FeasibilityCheck = false;

	DIFFIndicatorStates states = States_Begin;
	DIFFIndicatorStates prevState = states;
	while(states != MaxIndicatorStates)
	{
		switch(states)
		{
		case States_Begin:
			if(!FeasibilityCheck)
			{
				prevState = states;
				states = Check_States_Feasibility;
			}
			else
			{	
				FeasibilityCheck = false;
				states = Check_Visibility;
			}
			break;
		case Both_NotVisible:
			states = Change_Value;
			
			break;
		case FirstVisible_SecondNotVisible:
			if(first > second)
			{
				m_GridList[SECOND_TABLE]->GoToCell(second, 0);
				states = EndState;
			}
			else
			{
				states = Change_Value;
			}
			break;
		case FirstNotVisible_SecondVisible:
			if(first < second)
			{
				m_GridList[FIRST_TABLE]->GoToCell(first, 0);
				states = EndState;
			}
			else
			{
				states = Change_Value;
			}
			break;
		case Both_Visible:
			states = Change_Value;
			break;
		case Check_States_Feasibility:
			if(!m_CompItr.GetValue())
			{
				m_CompItr.Reset(true);
				states = EndState;
			}
			else
			{ 				
				states = prevState;
			}
			FeasibilityCheck = true;
			break;
		case Change_Value:
			if(m_CompItr.Previous())
			{
				m_CompItr--;
				states = Value_Changed;
			}
			else
			{
				states = EndState;
			}
			break;
		case Value_Changed:
			if(!FeasibilityCheck)
			{
				prevState = states;
				states = Check_States_Feasibility;
			}
			else
			{
				FeasibilityCheck = false;
				int r1 = m_CompItr.GetValue()->Row1;
				int r2 = m_CompItr.GetValue()->Row2;
				c1.Set(r1, 0);
				if(m_GridToContainerMap[0].FindFirst(c1, g1))
				{
					first = g1.X();
				}
				c2.Set(r2, 0);
				if(m_GridToContainerMap[1].FindFirst(c2, g2))
				{
					second = g2.X();
				}				
				if(first < second)
				{
					m_GridList[FIRST_TABLE]->GoToCell(first, 0);
				}
				else
				{
					m_GridList[SECOND_TABLE]->GoToCell(second, 0);
				}
				states = EndState;
			}
			break;
		case Check_Visibility:
			{
				int r1 = m_CompItr.GetValue()->Row1;
				int r2 = m_CompItr.GetValue()->Row2;
				c1.Set(r1, 0);
				if(m_GridToContainerMap[0].FindFirst(c1, g1))
				{
					first = g1.X();
				}
				c2.Set(r2, 0);
				if(m_GridToContainerMap[1].FindFirst(c2, g2))
				{
					second = g2.X();
				}

				FirstVisible = m_GridList[FIRST_TABLE]->IsRowVisible(first);
				SecondVisible = m_GridList[SECOND_TABLE]->IsRowVisible(second);
				if(FirstVisible &&   SecondVisible)
				{
					states = Both_Visible;
				}
				else if(FirstVisible)
				{
					states = FirstVisible_SecondNotVisible;
				}
				else if(SecondVisible)
				{
					states = FirstNotVisible_SecondVisible;
				}
				else
				{
					states = Both_NotVisible;
				}
			}
			break;
		case EndState:
			FeasibilityCheck = false;
			MoveIndicators();
			states = MaxIndicatorStates;
			break;
		default:
			states = MaxIndicatorStates;
			break;
		}
	}
}

void GridTableCompareView::mShowIndicators(int Y, int TableNum)
{
	XLObservedData data;
	data.Data = Y;
	data.Action = ActionShow;
	if(TableNum == FIRST_TABLE)
	{
		data.EventType = GRID_POINTER_MOVE_LEFT;
	}
	else
	{
		data.EventType = GRID_POINTER_MOVE_RIGHT;
	}
	Notify(&data, data.EventType);
}
void GridTableCompareView::mHideIndicators(int TableNum)
{
	XLObservedData data;
	data.Action = ActionHide;
	if(TableNum == FIRST_TABLE)
	{
		data.EventType = GRID_POINTER_MOVE_LEFT;
	}
	else
	{
		data.EventType = GRID_POINTER_MOVE_RIGHT;
	}
	
	Notify(&data, data.EventType);
}

void GridTableCompareView::MoveIndicators(bool ForceHide)
{
	if(ForceHide)
	{
		mHideIndicators(FIRST_TABLE);
		mHideIndicators(SECOND_TABLE);
		return;
	}

	if(!m_CompItr.IsValid())
	{
		return;
	}
	if(m_GridList[FIRST_TABLE])
	{
		int R1 = m_CompItr.GetValue()->Row1;
		CellIdentifier gr1;
		CellIdentifier cr1(R1, 0);
		if(m_GridToContainerMap[FIRST_TABLE].FindFirst(cr1, gr1))
		{ 		
			if(m_GridList[FIRST_TABLE]->IsRowVisible(gr1.X()))
			{
				RECT rect;
				m_GridList[FIRST_TABLE]->GetCellRect(gr1.X(), 0, &rect);
				int Y = rect.top + XLUtility::GetInstance()->GetTableY();
				mShowIndicators(Y, FIRST_TABLE);
			}
			else
			{
				mHideIndicators(FIRST_TABLE);
			}
		}
	}
	if(m_GridList[SECOND_TABLE])
	{
		int R2 = m_CompItr.GetValue()->Row2;
		CellIdentifier gr2;
		CellIdentifier cr2(R2, 0);
		if(m_GridToContainerMap[SECOND_TABLE].FindFirst(cr2, gr2))
		{ 		
			if(m_GridList[SECOND_TABLE]->IsRowVisible(gr2.X()))
			{
				RECT rect;
				m_GridList[SECOND_TABLE]->GetCellRect(gr2.X(), 0, &rect);
				int Y = rect.top + XLUtility::GetInstance()->GetTableY();
				mShowIndicators(Y, SECOND_TABLE);
			}
			else
			{
				mHideIndicators(SECOND_TABLE);
			}
		}
	}
}

void GridTableCompareView::FormatCells()
{
	for(int j = 0; j < m_GridList.size(); j++)
	{
		FormatATable(j);
	}
}




void GridTableCompareView::FormatATable(int tableI)
{
	if(tableI < 0 || tableI >= 3)
		return ;
	if(!m_GridList[tableI] || !m_ContainerList[tableI])
		return;
	COLORREF sameColor = MEDAQUAMARCOL;
	COLORREF txtSameColor = BLUECOL;

	COLORREF diffColor = DARKREDCOL;
    COLORREF tstDiffColor = BLUECOL;

	CellIdentifier gC;
	CellIdentifier ContC;
	int dbRow = 0;

	RowComparisonIterator rowCompItr(&m_iChangedCols);
	while(rowCompItr.GetValue())
	{
		if(tableI == 0)
		{
			dbRow = rowCompItr.GetValue()->Row1;
		}
		else
		{
			dbRow = rowCompItr.GetValue()->Row2;
		}
		
		for(int k = 0; k < m_ContainerList[tableI]->Col(); k++)
		{
			ContC.Set(dbRow, k);
			m_GridToContainerMap[tableI].FindFirst(ContC, gC);
			if(tableI == FIRST_TABLE)
			{
				int c2 = 0;				
				if(rowCompItr.GetValue()->FindC1(k, c2))
				{					
					m_GridList[tableI]->SetItemBkColour(gC.X(), gC.Y(), diffColor);
					m_GridList[tableI]->SetItemFgColour(gC.X(), gC.Y(), tstDiffColor);
				   
				}
				else
				{
				   m_GridList[tableI]->SetItemBkColour(gC.X(), k, sameColor);
				   m_GridList[tableI]->SetItemFgColour(gC.X(), k, txtSameColor);
				}
			}
			else
			{
				int c1 = 0;
				if(rowCompItr.GetValue()->FindC2(k, c1))
				{
					m_GridList[tableI]->SetItemBkColour(gC.X(), gC.Y(), diffColor);
					m_GridList[tableI]->SetItemFgColour(gC.X(), gC.Y(), tstDiffColor);
				   
				}
				else
				{
				   m_GridList[tableI]->SetItemBkColour(gC.X(), k, sameColor);
				   m_GridList[tableI]->SetItemFgColour(gC.X(), k, txtSameColor);
				}
			}
		}		
		rowCompItr++;
	}
}

void GridTableCompareView::ReCreateGridList()
{
	m_CompItr.Reset();
	ResetTables();
	CreateTable();
}


int GridTableCompareView::NumberofDrawableTables()
{
	int ret = 0;
	for(int i = 0; i < m_ContainerList.size(); i++)
	{
		if(m_ContainerList[i])
		{
			ret++;
		}
	}
	return ret;
}


void GridTableCompareView::ResetTables()
{
	for(int i = 0; i < m_GridList.size(); i++)
	{
		if(m_GridList[i])
			delete m_GridList[i];
		m_GridList[i] = 0;
	}
}


void GridTableCompareView::ResetContainers()
{
	for(int i = 0; i < m_ContainerList.size(); i++)
	{
		if(m_ContainerList[i])
			delete m_GridList[i];
		m_ContainerList[i] = 0;
	}
}



CompareResult GridTableCompareView::CreateDiffTable()
{
	return Compare(XLTableParamDiffTable);
}
					
void GridTableCompareView::mDrawDiffTable()
{
	int y = 0;
	if(IsReady() && m_ContainerList[0] && m_ContainerList[1])
	{
		COLORREF sameColor = MEDAQUAMARCOL;
		COLORREF txtSameColor = BLUECOL;

		COLORREF diffColor = DARKREDCOL;
	    COLORREF tstDiffColor = DARKSLATEGRAYCOL;

		int row1 = 0;	
		int row2 = 0;
		CreateGrid(m_GridList[0], m_ContainerList[0], 0);
		CreateGrid(m_GridList[1], m_ContainerList[1], 1);
		EnableHistory(false);
		RowComparisonIterator rowCompItr(&m_iChangedCols);
		while(!rowCompItr.IsEnd())
		{
			int databaseRow1 = rowCompItr.GetValue()->Row1;
			int databaseRow2 = rowCompItr.GetValue()->Row2;
			y = 0;
			for(int k = 0; k < m_ContainerList[0]->Col(); k++)
			{
				m_fillGrid(m_GridList[0], m_ContainerList[0], databaseRow1, k, row1, k);
			}
			row1++;

			y = 0;
			for(int k = 0; k < m_ContainerList[1]->Col(); k++)
			{
				m_fillGrid(m_GridList[1], m_ContainerList[1], databaseRow2, k, row2, k);
				
			}
			row2++;
			rowCompItr++;
		}
		FormatATable(0);
		FormatATable(1);
		EnableHistory(true);
	}
}
void GridTableCompareView::mDrawSameTable()
{
	if(IsReady() && m_ContainerList[0] && m_ContainerList[1])
	{
		//MoveIndicators(true);
		int numTables = NumberofDrawableTables();
		XL_UTIL->VerticallyDivideArea(numTables, m_Partitions, m_iTableWidth);

		int row=0;
		CreateGrid(m_GridList[0], m_ContainerList[0], 0);
		CreateGrid(m_GridList[1], m_ContainerList[1], 1);
		EnableHistory(false);
		for(int j = 0; j < m_Comparator.UnChangedRows().size(); j++)
		{
			int databaseRow1 = m_Comparator.UnChangedRows()[j].first;
			int databaseRow2 = m_Comparator.UnChangedRows()[j].second;

			if(databaseRow1 != INVALIDROW)
			{
				for(int k = 0; k < m_ContainerList[0]->Col(); k++)
				{
					m_fillGrid(m_GridList[0], m_ContainerList[0], databaseRow1,row, k);
					
				}
			}
			if(databaseRow2 != INVALIDROW)
			{			
				for(int k = 0; k < m_ContainerList[1]->Col(); k++)
				{
					m_fillGrid(m_GridList[1], m_ContainerList[1], databaseRow2,row, k);	   				
				}
			}
			row++;
		}
		EnableHistory(true);
		MoveIndicators(true);
	}
}

void GridTableCompareView::Notify(XLObservedData* data, XLEventType condition)
{
	XLCtrlSubject::Notify(data, condition);
	if(condition == GRID_VSCROLL)
	{
		MoveIndicators();
	}
}

void GridTableCompareView::CreateSameTable()
{
    Compare(XLTableParamSameTable);
}
	
void GridTableCompareView::CreateAllTable()
{  
	Compare(XLTableParamFullTable);
}


void GridTableCompareView::ICopyRightToLeft()// Arrow pointing right, shifting right to left
{
	 COLORREF Whitecol = WHITECOL;
     COLORREF Blackcol = BLACKCOL;

	if(GetCurrentObserver() == m_GridList[1])
	{
		int cRow = m_GridList[1]->GetMouseSelectedCell().row;
		int cCol = m_GridList[1]->GetMouseSelectedCell().col;
		CellIdentifier gridCell(cRow, cCol);
		CellIdentifier containerCell;

		if(!m_GridToContainerMap[1].FindSecond(gridCell, containerCell))
		{
			return;
		}
		
		RowComparison* rowCompCell = m_iChangedCols.FindRow2(containerCell.X());
		int y = 0;
		int count = 0;
		if(rowCompCell)
		{
			int r1 =  rowCompCell->Row1;
			int r2 =  rowCompCell->Row2;
			int c1 =  0;
			int c2 =  containerCell.Y();

			if(!rowCompCell->FindC2(c2, c1))
			{
				return;
			}

			InterChangeData(m_ContainerList[1], m_ContainerList[0], r2, c2, r1, c1);

			CellIdentifier FirstContainerCell(r1, c1);
			CellIdentifier FirstGridCell;

			if(!m_GridToContainerMap[0].FindFirst(FirstContainerCell, FirstGridCell))
			{
				return;
			}
			int gr1 = FirstGridCell.X();
			int gc1 = FirstGridCell.Y();

			m_fillGrid(m_GridList[0], m_ContainerList[0], r1, c1, gr1, gc1);

			rowCompCell->Erase(c1, c2);

			if(rowCompCell->SizeCol() == 0)
			{
				m_GridList[1]->SetRowBkColor(cRow ,Whitecol);
				m_GridList[1]->SetRowFgColor(cRow , Blackcol);
				m_GridList[0]->SetRowBkColor(gr1 , Whitecol);
				m_GridList[0]->SetRowFgColor(gc1 , Blackcol);
				m_GridList[1]->RedrawRow(cRow);
			    m_GridList[0]->RedrawRow(gr1);

				m_iChangedCols.Delete(r1, r2);	   
			}
			else
			{
				GridSameCol(1,cRow,cCol,0,gr1,gc1);
			}
		}
		
	}
}


void GridTableCompareView::ICopyLeftToRight()// Arrow pointing left, shifting left to right
{
	COLORREF Whitecol = WHITECOL;
    COLORREF Blackcol = BLACKCOL;

	if(GetCurrentObserver() == m_GridList[0])
	{
		int cRow = m_GridList[0]->GetMouseSelectedCell().row;
		int cCol = m_GridList[0]->GetMouseSelectedCell().col;
		CellIdentifier gridCell(cRow, cCol);
		CellIdentifier containerCell;

		if(!m_GridToContainerMap[0].FindSecond(gridCell, containerCell))
		{
			return;
		}
		
		RowComparison* rowCompCell = m_iChangedCols.FindRow1(containerCell.X());
		int y = 0;
		int count = 0;
		if(rowCompCell)
		{
			int r1 =  rowCompCell->Row1;
			int r2 =  rowCompCell->Row2;
			int c1 =  containerCell.Y();
			int c2 =  0;

			if(!rowCompCell->FindC1(c1, c2))
			{
				return;
			}

			InterChangeData(m_ContainerList[0], m_ContainerList[1], r1, c1, r2, c2);

			CellIdentifier SecondContainerCell(r2, c2);
			CellIdentifier SecondGridCell;

			if(!m_GridToContainerMap[1].FindFirst(SecondContainerCell, SecondGridCell))
			{
				return;
			}

			int gr2 = SecondGridCell.X();
			int gc2 = SecondGridCell.Y();

			m_fillGrid(m_GridList[1], m_ContainerList[1], r2, c2, gr2, gc2);

			rowCompCell->Erase(c1, c2);

			if(rowCompCell->SizeCol() == 0)
			{
				m_GridList[1]->SetRowBkColor(cRow ,Whitecol);
				m_GridList[1]->SetRowFgColor(cRow , Blackcol);
				m_GridList[0]->SetRowBkColor(gr2 , Whitecol);
				m_GridList[0]->SetRowFgColor(gc2 , Blackcol);
				m_GridList[1]->RedrawRow(cRow);
			    m_GridList[0]->RedrawRow(gr2);

				m_iChangedCols.Delete(r1, r2);	   
			}
			else
			{
				GridSameCol(0,cRow,cCol,1,gr2,gc2);
			}
		}
		
	}
}


void GridTableCompareView::OnCopyFromRightToLeft(int r1, int r2, int c1, int c2)
{
	if((r1 == -1) || (r2 == -1) || (c1 == -1) || (c2 == -1))
	{
		ICopyRightToLeft();
	}
	else
	{
		SelectionCopyRightToLeft(r1, r2, c1, c2);
	}
}

void GridTableCompareView::OnCopyFromLeftToRight(int r1, int r2, int c1, int c2)
{
	if((r1 == -1) || (r2 == -1) || (c1 == -1) || (c2 == -1))
	{
		ICopyLeftToRight();
	}
	else
	{
		SelectionCopyLeftToRight(r1, r2, c1, c2);
	}
}

void GridTableCompareView::LineCopyLeftToRight() //left arrow
{
	if(m_CompItr.IsEnd())
	{
		return;
	}

	
	RowComparison* rc = m_CompItr.GetValue();

	if(rc)
	{
		int r1 = m_CompItr.GetValue()->Row1;
		int r2 = m_CompItr.GetValue()->Row2;

		CellIdentifier containerCell1, containerCell2;
		CellIdentifier gridCell2(r2, 0);

		ColCompareItr citr(rc, true);
		while(!citr.IsEnd())
		{
			int c1 = citr.GetFValue();
			int c2 = 0;
			if(rc->FindC1(c1, c2))
			{
				InterChangeData(m_ContainerList[0], m_ContainerList[1], r1, c1, r2, c2);
				containerCell2.Set(r2, c2);
				if(m_GridToContainerMap[1].FindFirst(containerCell2, gridCell2))
				{
					m_fillGrid(m_GridList[1], m_ContainerList[1], r2, c2, gridCell2.X(), gridCell2.Y());
				}
			}
			
			citr++;
			
		}
		CellIdentifier gridCell1;
		
		for (int j =0; j< m_ContainerList[FIRST_TABLE]->Col(); j++)	
		{
			containerCell1.Set(r1, j);
			if(!m_GridToContainerMap[0].FindFirst(containerCell1, gridCell1))
			{
				continue;
			}
			GridSameCol(0,gridCell1.X(),gridCell1.Y(),1,-1,-1);
		}
		for (int j =0; j< m_ContainerList[SECOND_TABLE]->Col(); j++)	
		{
			containerCell2.Set(r2, j);
			if(!m_GridToContainerMap[1].FindFirst(containerCell2, gridCell2))
			{
				continue;
			}
			GridSameCol(1,gridCell2.X(),gridCell2.Y(),0,-1,-1);
		}
		m_CompItr.DeleteCurrValue();
	}

	
}

void GridTableCompareView::LineCopyRightToLeft() //Right arrow	    from right to left
{
    if(m_CompItr.IsEnd())
	{
		return;
	}

	
	RowComparison* rc = m_CompItr.GetValue();


	if(rc)
	{
		int r1 = rc->Row1;
		int r2 = rc->Row2;

		CellIdentifier containerCell1, containerCell2;
		CellIdentifier gridCell1(r1, 0);
		


		ColCompareItr citr(rc, false);
		while(!citr.IsEnd())
		{
			int c1 = 0;
			int c2 = citr.GetSValue();
			if(rc->FindC2(c2, c1))
			{
				InterChangeData(m_ContainerList[1], m_ContainerList[0], r2, c2, r1, c1);
				containerCell1.Set(r1, c1);
				if(m_GridToContainerMap[0].FindFirst(containerCell1, gridCell1))
				{
					m_fillGrid(m_GridList[0], m_ContainerList[0], r1, c1, gridCell1.X(), gridCell1.Y());
				}
			}
			
			citr++;
			
		}
		CellIdentifier gridCell2;
		for (int j =0; j< m_ContainerList[FIRST_TABLE]->Col(); j++)	
		{
			containerCell1.Set(r1, j);
			if(!m_GridToContainerMap[0].FindFirst(containerCell1, gridCell1))
			{
				continue;
			}
			GridSameCol(0,gridCell1.X(),gridCell1.Y(),1,-1,-1);
		}
		for (int j =0; j< m_ContainerList[SECOND_TABLE]->Col(); j++)	
		{
			containerCell2.Set(r2, j);
			if(!m_GridToContainerMap[1].FindFirst(containerCell2, gridCell2))
			{
				continue;
			}
			GridSameCol(1,gridCell2.X(),gridCell2.Y(),1,-1,-1);
		}
		m_CompItr.DeleteCurrValue();
	}
}


void GridTableCompareView::SelectionCopyLeftToRight(int r1, int r2, int c1, int c2)
{
	CellIdentifier gr1, gr2, con1, con2;
	COLORREF Whitecol = WHITECOL;
    COLORREF Blackcol = BLACKCOL;

	for(int i = r1; i <= r2; i++)
	{
		gr1.Set(i, 0);
		if(!m_GridToContainerMap[0].FindSecond(gr1, con1))
		{
			continue;
		}
		RowComparison* rc =  m_iChangedCols.FindRow1(con1.X());
		if(rc)
		{
			for(int j = c1; j <= c2; j++)
			{
				gr1.Set(i, j);
				if(!m_GridToContainerMap[0].FindSecond(gr1, con1))
				{
					continue;
				}
				int cc2 = 0;
				if(rc->FindC2(con1.Y(), cc2))
				{
					InterChangeData(m_ContainerList[0], m_ContainerList[1],	con1.X(), con1.Y(), rc->Row2, cc2);
					con2.Set(rc->Row2, cc2);
					if(m_GridToContainerMap[1].FindFirst(con2, gr2))
					{
						m_fillGrid(m_GridList[1], m_ContainerList[1],  con2.X(), con2.Y(), gr2.X(), gr2.Y());
						rc->Erase(con1.Y(), cc2);
						if(rc->SizeCol() == 0)
						{
							m_GridList[1]->SetRowBkColor(gr2.X() , Whitecol);
							m_GridList[1]->SetRowFgColor(gr2.X() , Blackcol);
							m_GridList[0]->SetRowBkColor(gr1.X() , Whitecol);
							m_GridList[0]->SetRowFgColor(gr1.X() , Blackcol);
							m_GridList[1]->RedrawRow(gr2.X());
							m_GridList[0]->RedrawRow(gr1.X());

							m_iChangedCols.Delete(r1, r2);	
						}
						else
						{
							GridSameCol(0, gr1.X(), gr1.Y(), 1, gr2.X(), gr2.Y());
						}
					}
					
				}				
			}
		}
	}
}
void GridTableCompareView::SelectionCopyRightToLeft(int r1, int r2, int c1, int c2)
{
	CellIdentifier gr1, gr2, con1, con2;
	COLORREF Whitecol = WHITECOL;
    COLORREF Blackcol = BLACKCOL;

	for(int i = r1; i <= r2; i++)
	{
		gr2.Set(i, 0);
		if(!m_GridToContainerMap[1].FindSecond(gr2, con2))
		{
			continue;
		}
		RowComparison* rc =  m_iChangedCols.FindRow2(con2.X());
		if(rc)
		{
			for(int j = c1; j <= c2; j++)
			{
				gr2.Set(i, j);
				if(!m_GridToContainerMap[1].FindSecond(gr2, con2))
				{
					continue;
				}
				int cc1 = 0;
				if(rc->FindC1(con2.Y(), cc1))
				{
					InterChangeData(m_ContainerList[1], m_ContainerList[0],	con2.X(), con2.Y(), rc->Row1, cc1);
					con1.Set(rc->Row1, cc1);
					if(m_GridToContainerMap[0].FindFirst(con1, gr1))
					{
						m_fillGrid(m_GridList[0], m_ContainerList[0],  con1.X(), con1.Y(), gr1.X(), gr1.Y());						
						rc->Erase(cc1, con2.Y());
						if(rc->SizeCol() == 0)
						{
							m_GridList[1]->SetRowBkColor(gr2.X() , Whitecol);
							m_GridList[1]->SetRowFgColor(gr2.X() , Blackcol);
							m_GridList[0]->SetRowBkColor(gr1.X() , Whitecol);
							m_GridList[0]->SetRowFgColor(gr1.X() , Blackcol);
							m_GridList[1]->RedrawRow(gr2.X());
							m_GridList[0]->RedrawRow(gr1.X());

							m_iChangedCols.Delete(r1, r2);	
						}
						else
						{
							GridSameCol(0, gr1.X(), gr1.Y(), 1, gr2.X(), gr2.Y());
						}
					}
					
				} 				
			}
		}
	}
}

void GridTableCompareView::InterChangeData(XLCellDataContainer* from, XLCellDataContainer* to, int r1, int c1, int r2, int c2)
{
	int tableId = 0;
	if(to == m_ContainerList[0])
	{
		tableId = 0;
	}
	else if(to == m_ContainerList[1])
	{
		tableId = 1;
	}
	if(from && to)
	{
		XLCellData* dataFrom = from->CellData(r1, c1);
		XLCellData* dataTo = to->CellData(r2, c2);

		RecordInHistory(tableId, CellIdentifier(r2, c2), dataTo);

		if(dataFrom && dataTo)
		{
			*dataTo = *dataFrom;
		}
	}
	NotifyFileChange(tableId);
	
}

bool GridTableCompareView::SaveInFiles()
{
	bool saveA = SaveFileA();
	bool saveB = SaveFileB(); 
	
	return (saveA & saveB);
}

bool GridTableCompareView::SaveFileA()
{
	CExcelDataExtraction e;
    if(e.SaveToExcelFormat(m_ContainerList[0],m_pathOne))
	{
		NotifyFileSaved();
		return true;
	}
	return false;
}
bool GridTableCompareView::SaveFileB()
{
	CExcelDataExtraction e;
	if(e.SaveToExcelFormat(m_ContainerList[1],m_pathTwo))
	{
		NotifyFileSaved();
		return true;
	}
	return false;
}

void GridTableCompareView::GridSameCol(int first, int r1, int col1, int second, int r2, int col2)
{
	COLORREF Whitecol = WHITECOL;
    COLORREF Blackcol = BLACKCOL;

	if((r1 != INVALIDROW)   &&	(col1 != INVALIDROW) && m_GridList[first])
	{
		m_GridList[first]->SetItemBkColour(r1, col1, Whitecol);
		m_GridList[first]->SetItemFgColour(r1, col1, Blackcol);
		m_GridList[first]->RedrawRow(r1);
	}
	if((r2 != INVALIDROW)   &&	(col2 != INVALIDROW) && m_GridList[second])
	{
		m_GridList[second]->SetItemBkColour(r2, col2, Whitecol);
		m_GridList[second]->SetItemFgColour(r2, col2, Blackcol);
		m_GridList[second]->RedrawRow(r2);
	}
			
}

void GridTableCompareView::UndoChanges()
{
	EnableHistory(false);
	HistDataPair* hP = RemoveRecentHistory();
	if(!hP)
	{
		EnableHistory(true);
		return;
	}

	CGridCtrl* grid = m_GridList[hP->TableID];
	if(grid)
	{
		CellIdentifier gridCell;
		XLCellDataContainer* container = m_ContainerList[hP->TableID];
		if(container)
		{
			XLCellData* cellData = container->CellData(hP->Cell);
			if(cellData)
			{
				*cellData = hP->Data;
			}
		}
		if(m_GridToContainerMap[hP->TableID].FindFirst(hP->Cell, gridCell))
		{
			m_fillGrid(grid, container, hP->Cell.X(), hP->Cell.Y(), gridCell.X(), gridCell.Y());
		}
		grid->RedrawRow(gridCell.X());
		//grid->GoToRow(gridCell.X());
	}
	Compare(m_CurrentState);
	EnableHistory(true);
}

void GridTableCompareView::SaveToDatabase(int r, int c, CString& value)
{
	//GridViewHelper::GetInstance()->SetGridView(this);
	//GridViewHelper::GetInstance()->SetDBParam(new SaveDBParam(r, c, value));
	//GridViewHelper::GetInstance()->RunProgramAsync(AsyncProgramType_SaveToDB);
	mSaveToDataBase(r,c, value);
}

void GridTableCompareView::SaveToTable(int r, int c, CString& value, int table)
{
	CCellID topLeft;
	CellIdentifier grid(r, c), cell;
	if(table == FIRST_TABLE)
	{
		if(m_GridToContainerMap[0].FindSecond(grid, cell))
		{
			XLCellData* cellData = m_ContainerList[0]->CellData(cell.X(), cell.Y());
			if(cellData)
			{
				RecordInHistory(0, cell, cellData);
				cellData->SetDataAndIndentifyType(value);
			}
			topLeft = m_GridList[0]->GetVisibleTopLeftCell();
		}
	}
	else if(table == SECOND_TABLE)
	{
		if(m_GridToContainerMap[1].FindSecond(grid, cell))
		{
			XLCellData* cellData = m_ContainerList[1]->CellData(cell.X(), cell.Y());
			if(cellData)
			{
				RecordInHistory(1, cell, cellData);
				cellData->SetDataAndIndentifyType(value);
			}
			topLeft = m_GridList[1]->GetVisibleTopLeftCell();
		}		
	}
	Compare(m_CurrentState);
	m_GridList[table]->GoToCell(r, c);
	NotifyFileChange(table);
}


void GridTableCompareView::mSaveToDataBase(int r, int c, CString& value)
{
	int table = 0;
	CCellID topLeft;
	CellIdentifier grid(r, c), cell;
	if(GetCurrentObserver() == m_GridList[0])
	{
		if(m_GridToContainerMap[0].FindSecond(grid, cell))
		{
			XLCellData* cellData = m_ContainerList[0]->CellData(cell.X(), cell.Y());
			if(cellData)
			{
				RecordInHistory(0, cell, cellData);
				cellData->SetDataAndIndentifyType(value);
			}
			topLeft = m_GridList[0]->GetVisibleTopLeftCell();
		}
		table = 0;
	}
	if(GetCurrentObserver() == m_GridList[1])
	{
		if(m_GridToContainerMap[1].FindSecond(grid, cell))
		{
			XLCellData* cellData = m_ContainerList[1]->CellData(cell.X(), cell.Y());
			if(cellData)
			{
				RecordInHistory(1, cell, cellData);
				cellData->SetDataAndIndentifyType(value);
			}
			topLeft = m_GridList[1]->GetVisibleTopLeftCell();
		}
		table = 1; 		
	}
	Compare(m_CurrentState);
	m_GridList[table]->GoToCell(r, c);
	NotifyFileChange(table);
}


void GridTableCompareView::NotifyFileChange(int tbl)
{
	if(tbl < 0 || tbl >= 2)
		return;
	XLObservedData data;
	if(tbl == 0)
	{
		data.Data = GRID_LEFT_FILE_CHANGED;
		Notify(&data, GRID_LEFT_FILE_CHANGED);
	}
	else
	{
		data.Data = GRID_RIGHT_FILE_CHANGED;
		Notify(&data, GRID_RIGHT_FILE_CHANGED);
	}
	
}

void GridTableCompareView::NotifyFileSaved()
{
	XLObservedData data;
	data.Data = GRID_FILE_SAVED;
	Notify(&data, GRID_FILE_SAVED);
}


