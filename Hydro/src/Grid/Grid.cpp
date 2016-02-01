/**
 * \file
 * \brief General Grid realization.
 *
 * \author Alexey Rybakov
 */

#include <mpi.h>
#include <fstream>
#include <cassert>
#include <vector>
#include "Lib/MPI/mpi.h"
#include "Grid.h"

namespace Hydro { namespace Grid {

/*
 * Constructors/destructors.
 */

/**
 * \brief Default constructor.
 */
Grid::Grid()
    : Blocks_p_(NULL),
      Blocks_Count_(0),
      Ifaces_p_(NULL),
      Ifaces_Count_(0),
      Layer_(0)
{
    Init_Timers();
}

/**
 * \brief Default destructor.
 */
Grid::~Grid()
{
    Deallocate_Blocks();
    Deallocate_Blocks_Pointers();
    Deallocate_Ifaces();
    Deallocate_Ifaces_Pointers();
}

/**
 * \brief Init timers.
 */
void Grid::Init_Timers()
{
    Timer_Shadow_Exchange_p_ = new Lib::MPI::Timer();
}

/*
 * Simple data functions and characteristics.
 */

/**
 * \brief Get block.
 *
 * \param[in] n - block number.
 *
 * \return
 * Block.
 */
Block *Grid::Get_Block(int n) const
{
    assert((n >= 0) && (n < Blocks_Count_));

    return Blocks_p_[n];
}

/**
 * \brief Get interface.
 *
 * \param[in] n - interface number.
 *
 * \return
 * Interface.
 */
Iface *Grid::Get_Iface(int n) const
{
    assert((n >= 0) && (n < Ifaces_Count_));

    return Ifaces_p_[n];
}

/**
 * \brief Get cells count.
 *
 * \return
 * Count of cells.
 */
int Grid::Cells_Count() const
{
    int c = 0;

    for (int i = 0; i < Blocks_Count(); i++)
    {
        c += Get_Block(i)->Cells_Count();
    }

    return c;
}

/**
 * \brief Get bytes count.
 *
 * \return
 * Count of bytes.
 */
long Grid::Bytes_Count() const
{
    long c = 0;

    for (int i = 0; i < Blocks_Count(); i++)
    {
        c += Get_Block(i)->Bytes_Count();
    }

    return c;
}

/**
 * \brief Get interface cells count.
 *
 * \return
 * Count of interface cells.
 */
int Grid::Iface_Cells_Count() const
{
    int c = 0;

    for (int i = 0; i < Blocks_Count(); i++)
    {
        c += Get_Block(i)->Iface_Cells_Count();
    }

    return c;
}

/**
 * \brief Get shadow cells count.
 *
 * \return
 * Count of shadow cells.
 */
int Grid::Shadow_Cells_Count() const
{
    int c = 0;

    for (int i = 0; i < Blocks_Count(); i++)
    {
        c += Get_Block(i)->Shadow_Cells_Count();
    }

    return c;
}

/**
 * \brief Get inner cells count.
 *
 * \return
 * Count of inner cells.
 */
int Grid::Inner_Cells_Count() const
{
    int c = 0;

    for (int i = 0; i < Blocks_Count(); i++)
    {
        c += Get_Block(i)->Inner_Cells_Count();
    }

    return c;
}

/**
 * \brief Get border cells count.
 *
 * \return
 * Count of border cells.
 */
int Grid::Border_Cells_Count() const
{
    return Cells_Count() - Inner_Cells_Count();
}

/**
 * \brief Get count of cell used in MPI exchange.
 *
 * \return
 * MPI cells count.
 */
int Grid::MPI_Cells_Count() const
{
    int c = 0;

    // Count of MPI cell is just count of cells in interfaces,
    // where nodes are from differet processes.
    for (int i = 0; i < Ifaces_Count(); i++)
    {
        Iface *p = Get_Iface(i);

        if (p->B()->Rank() != p->NB()->Rank())
        {
            c += p->Cells_Count();
        }
    }

    return c * HYDRO_GRID_SHADOW_DEPTH;
}

/*
 * Allocate/deallocate blocks.
 */

/**
 * \brief Allocate blocks pointers.
 *
 * \param[in] count - blocks count
 *
 * \return
 * true - if blocks array is created (array of blocks pointers)<
 * false - if blocks array is not created.
 */
bool Grid::Allocate_Blocks_Pointers(int count)
{
    Deallocate_Blocks();
    Blocks_p_ = new Block *[count];
    Blocks_Count_ = count;

    return Blocks_p_ != NULL;
}

/**
 * \brief Deallocate blocks.
 */
void Grid::Deallocate_Blocks()
{
    for (int i = 0; i < Blocks_Count(); i++)
    {
        Block *p = Get_Block(i);

        if (p != NULL)
        {
            delete p;
        }
    }
}

/**
 * \brief Deallocate blocks pointers.
 */
void Grid::Deallocate_Blocks_Pointers()
{
    if (Blocks_p_ != NULL)
    {
        delete Blocks_p_;
        Blocks_p_ = NULL;
        Blocks_Count_ = 0;
    }
}

/**
 * \brief Allocate interfaces pointers.
 *
 * \param[in] count - count of interfaces
 *
 * \return
 * true - if interfaces pointers are allocated,
 * false - if interfaces pointers are not allocated.
 */
bool Grid::Allocate_Ifaces_Pointers(int count)
{
    Deallocate_Ifaces();
    Ifaces_p_ = new Iface *[count];
    Ifaces_Count_ = count;

    return Ifaces_p_ != NULL;
}

/**
 * \brief Deallocate interfaces.
 */
void Grid::Deallocate_Ifaces()
{
    for (int i = 0; i < Ifaces_Count(); i++)
    {
        Iface *p = Get_Iface(i);

        if (p != NULL)
        {
            delete p;
        }
    }
}

/**
 * \brief Deallocate interfaces pointers.
 */
void Grid::Deallocate_Ifaces_Pointers()
{
    if (Ifaces_p_ != NULL)
    {
        delete Ifaces_p_;
        Ifaces_p_ = NULL;
        Ifaces_Count_ = 0;
    }
}

/*
 * Load/save Grid.
 */

/**
 * \brief Load data from GEOM format.
 *
 * \param[in] name - name of data
 * \param[in] ranks_count - count of ranks
 *
 * \return
 * true - if grid is loaded,
 * false - in other cases.
 */
bool Grid::Load_GEOM(const string name,
                     int ranks_count)
{
    string name_pfg = name + ".pfg";
    string name_ibc = name + ".ibc";
    ifstream file_pfg, file_ibc;

    file_pfg.open(name_pfg.c_str());
    if (!file_pfg.is_open())
    {
        cout << "Err: Cannot open file: " << name_pfg << endl;

        return false;
    }

    file_ibc.open(name_ibc.c_str());
    if (!file_ibc.is_open())
    {
        cout << "Err: Cannot open file: " << name_ibc << endl;

        return false;
    }

    int blocks_count;

    // Read blocks count and allocate memory for pointers.
    file_pfg >> blocks_count;
    Allocate_Blocks_Pointers(blocks_count);

    // Read all blocks.
    for (int i = 0; i < blocks_count; i++)
    {
        int i_size, j_size, k_size;

        // Read block sizes and create block.
        file_pfg >> i_size >> j_size >> k_size;
        Blocks_p_[i] = new Block(this, i, i_size - 1, j_size - 1, k_size - 1);
    }

    // Blocks balancing.
    Set_Blocks_Ranks_Cells_Balancing(ranks_count);

    // Allocate memory for blocks.
    for (int i = 0; i < blocks_count; i++)
    {
        Block *p = Get_Block(i);

        if (p->Is_Active())
        {
            p->Allocate_Memory();
        }
    }

    if (!Load_GEOM_Ifaces(file_ibc))
    {
        cout << "Err: Interfaces loading failed." << endl;
    }
    Set_Ifaces_To_Facets();

    // Close files.
    file_pfg.close();
    file_ibc.close();

    return true;
}

/**
 * \brief Load Grid interfaces.
 *
 * \param[in] s - stream
 */
bool Grid::Load_GEOM_Ifaces(ifstream &s)
{
    string tmp;
    int ifaces_count, pos;

    // Pass two strings.
    getline(s, tmp);
    getline(s, tmp);

    // Read interfaces count and set up pointers.
    s >> ifaces_count;
    Allocate_Ifaces_Pointers(ifaces_count);

    // First set null pointers.
    for (int i = 0; i < ifaces_count; i++)
    {
        Ifaces_p_[i] = NULL;
    }

    // Read all interfaces.
    pos = 0;
    for (int iter = 0; iter < ifaces_count; iter++)
    {
        int id, bid, i0, i1, j0, j1, k0, k1, nid;

        // Read interface parameters and create interface.
        s >> id >> bid >> i0 >> i1 >> j0 >> j1 >> k0 >> k1 >> nid;

        // We are going to write interface to position "pos".
        // But if there is interface with the same id before,
        // then we have to write current interface after it.

        // Try to search i.
        int cur_pos = -1;
        for (int i = 0; i < ifaces_count; i++)
        {
            Iface *p = Get_Iface(i);

            if (p != NULL)
            {
                if (p->Id() == id)
                {
                    cur_pos = i;

                    break;
                }
            }
        }

        if (cur_pos != -1)
        {
            // We have this id before (and we have place for second interface with the same id).
            cur_pos++;
        }
        else
        {
            // Use position "pos".
            cur_pos = pos;
            pos += 2;
        }

        Ifaces_p_[cur_pos] = new Iface(id,
                                       Get_Block(bid - 1),
                                       i0 - 1, i1 - 1, j0 - 1, j1 - 1, k0 - 1, k1 - 1,
                                       Get_Block(nid - 1));
    }

    return true;
}

/**
 * \brief Set ifaces pointers to facets.
 */
void Grid::Set_Ifaces_To_Facets()
{
    // Analyze each iface.
    for (int i = 0; i < Ifaces_Count(); i++)
    {
        Iface *i_p = Get_Iface(i);
        Block *b_p = i_p->B();
        Facet *f_p = b_p->Get_Facet(i_p->Direction());

        f_p->Set_Iface(i_p);
    }
}

/**
 * \brief Create Descartes grid with real sizes.
 *
 * We create grid where i - is x direction, j - y direction, k - z direction.
 * X interval is [0, i_real_size],
 * Y interval is [0, j_real_size],
 * Z interval is [0, k_real_size].
 *
 * \param[in] i_size - cells count in i direction
 * \param[in] j_size - cells count in j direction
 * \param[in] k_size - cells count in k direction
 * \param[in] i_real_size - size in meters in i direction
 * \param[in] j_real_size - size in meters in j direction
 * \param[in] k_real_size - size in meters in k direction
 */
void Grid::Create_Solid_Descartes(int i_size,
                                  int j_size,
                                  int k_size,
                                  double i_real_size,
                                  double j_real_size,
                                  double k_real_size)
{
    // Our grid has 1 block and no interfaces.
    Allocate_Blocks_Pointers(1);
    Blocks_p_[0] = new Block(this, 0, i_size, j_size, k_size);
    Blocks_p_[0]->Create_Solid_Descartes(i_real_size, j_real_size, k_real_size);
}

/*
 * Information.
 */

/**
 * \brief Print Grid information.
 *
 * \param[in] os - stream
 * \param[in] grid_ - Grid pointer
 */
ostream &operator<<(ostream &os,
                    const Grid *grid_p)
{
    int blocks_count = grid_p->Blocks_Count();
    int ifaces_count = grid_p->Ifaces_Count();

    os << "Grid(b" << blocks_count << ",i" << ifaces_count << "):" << endl;

    for (int i = 0; i < blocks_count; i++)
    {
        Block *p = grid_p->Get_Block(i);

        if (p != NULL)
        {
            os << p;
        }
    }

    for (int i = 0; i < ifaces_count; i++)
    {
        Iface *p = grid_p->Get_Iface(i);

        if (p != NULL)
        {
            os << p;
        }
    }
}

/*
 * Blocks ranks balancing.
 */

/**
 * \brief Circular distribution of blocks ranks.
 *
 * \param[in] ranks - ranks count
 */
void Grid::Set_Blocks_Ranks_Circular_Distribution(int ranks_count)
{
    for (int i = 0; i < Blocks_Count(); i++)
    {
        Get_Block(i)->Set_Rank(i % ranks_count);
    }
}

/**
 * \brief Balancing of blocks ranks.
 *
 * \param[in] ranks_count - count of ranks
 */
void Grid::Set_Blocks_Ranks_Cells_Balancing(int ranks_count)
{
    int blocks_count = Blocks_Count();

    if (Is_Empty())
    {
        return;
    }

    // Allocate counters of cells count for each rank.
    int *ranks_cells = new int[ranks_count];

    // Zero all counters.
    for (int i = 0; i < ranks_count; i++)
    {
        ranks_cells[i] = 0;
    }

    // Reset blocks ranks.
    for (int i = 0; i < blocks_count; i++)
    {
        Get_Block(i)->Set_Rank(-1);
    }

    // Now we put the biggest block without rank to the most empty rank.
    for (int iter = 0; iter < blocks_count; iter++)
    {
        int big_block_num = -1;
        int big_block_size = 0;

        for (int i = 0; i < blocks_count; i++)
        {
            Block *block_p = Get_Block(i);

            if (block_p->Rank() == -1)
            {
                int block_size = block_p->Cells_Count();

                if (block_size > big_block_size)
                {
                    big_block_num = i;
                    big_block_size = block_size;
                }
            }
        }

        int emp_rank_num = 0;

        for (int i = 1; i < ranks_count; i++)
        {
            if (ranks_cells[i] < ranks_cells[emp_rank_num])
            {
                emp_rank_num = i;
            }
        }

        // Put big_block_num block to emp_rank_num rank.
        Get_Block(big_block_num)->Set_Rank(emp_rank_num);
        ranks_cells[emp_rank_num] += big_block_size;
    }

    delete ranks_cells;
}

/*
 * Calculations.
 */

/**
 * \brief Calculate single iteration.
 */
void Grid::Calculate_Iteration()
{
    // Set predefined values to buffers.
    for (int i = 0; i < Ifaces_Count(); i++)
    {
        Iface *p = Get_Iface(i);

        if (p->Is_MPI())
        {
            if (p->Is_BActive())
            {
                p->Set_Buffer_Value(0.0);
            }
            else
            {
                p->Set_Buffer_Value(1.0);
            }
        }
    }

    // Exchange.
    Ifaces_MPI_Data_Exchange();

    // Check buffers.
    for (int i = 0; i < Ifaces_Count(); i++)
    {
        Iface *p = Get_Iface(i);

        if (p->Is_MPI())
        {
            if (p->Is_BActive())
            {
                assert(p->Check_Buffer_Value(1.0, 0.001));
            }
        }
    }
}

/**
 * \brief Calculate iterations.
 *
 * \param[in] n - iterations number
 */
void Grid::Calculate_Iterations(int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << setw(3) << Lib::MPI::Rank() << " | iter: " << setw(5) << i;
        Calculate_Iteration();
        cout << " : done" << endl;
    }
}

/**
 * \brief MPI data exchange for interfaces.
 */
void Grid::Ifaces_MPI_Data_Exchange()
{
    Timer_Shadow_Exchange()->Start();

    MPI_Request reqs[1000];
    MPI_Status stats[1000];
    int reqs_count = 0;
    int i = 0;

    // Process all interfaces.
    while (i < Ifaces_Count())
    {
        Iface *p = Get_Iface(i);

        if (p->Is_BActive())
        {
            if (p->Is_NActive())
            {
                // Both blocks are active.
                // We can replace data without MPI.

                i += 2;
            }
            else
            {
                // Self block is active, neighbour is not.
                // We have to receive data from neighbour block process.

                MPI_Irecv(p->MPI_Buffer(), p->Buffer_Doubles_Count(), MPI_FLOAT,
                          p->NB()->Rank(), p->Id(), MPI_COMM_WORLD, &reqs[reqs_count++]);
                i++;
            }
        }
        else
        {
            if (p->Is_NActive())
            {
                // Neighbour block is active, self is not.
                // We have to send data to self block process.
                MPI_Isend(p->MPI_Buffer(), p->Buffer_Doubles_Count(), MPI_FLOAT,
                          p->B()->Rank(), p->Id(), MPI_COMM_WORLD, &reqs[reqs_count++]);
                i++;
            }
            else
            {
                // Self and neighbour blocks are not active.
                // Nothing to do.
                i += 2;
            }
        }
    }

    // Wait all requests.
    MPI_Waitall(reqs_count, reqs, stats);

    Timer_Shadow_Exchange()->Stop();
}

/*
 * Information.
 */

/**
 * \brief Print timers.
 *
 * \param[in] os - stream
 */
void Grid::Print_Timers(ostream &os)
{
    os << "Timers:" << endl;
    os << "  MPI_Shadow_Exchange : " << Timer_Shadow_Exchange()->Time() << endl;
}

/**
 * \brief Print statistics.
 *
 * \param[in] os - stream
 */
void Grid::Print_Statistics(ostream &os)
{
    int cc = Cells_Count();
    int ifcc = Iface_Cells_Count();
    int scc = Shadow_Cells_Count();
    int icc = Inner_Cells_Count();
    int bcc = Border_Cells_Count();
    int mcc = MPI_Cells_Count();

    os << "Statistics:" << endl;

    /*
     * Main characteristics of grid.
     */

    os << "  Blocks Count         : " << setw(8) << Blocks_Count() << endl;
    os << "  Ifaces Count         : " << setw(8) << Ifaces_Count() << endl;
    os << "  Cells Count          : " << setw(8) << cc << endl;
    os << "  MBytes Count         : " << setw(8) << Bytes_Count() / (1024 * 1024) << endl;
    os << "  Inner Cells Count    : " << setw(8) << icc << endl;
    os << "  Inner Cells Percent  : " << setw(8) << setprecision(2) << fixed
                                      << (100.0 * icc / cc) << " %" << endl;
    os << "  Border Cells Count   : " << setw(8) << bcc << endl;
    os << "  Border Cells Percent : " << setw(8) << setprecision(2) << fixed
                                      << (100.0 * bcc / cc) << " %" << endl;
    os << "  Iface Cells Count    : " << setw(8) << ifcc << endl;
    os << "  Iface Cells Percent  : " << setw(8) << setprecision(2) << fixed
                                      << (100.0 * ifcc / cc) << " %" << endl;
    os << " Shadow Cells Count    : " << setw(8) << scc << endl;
    os << " Shadow Cells Percent  : " << setw(8) << setprecision(2) << fixed
                                      << (100.0 * scc / cc) << " %" << endl;
    os << "     MPI Cells Count   : " << setw(8) << mcc << endl;
    os << "     MPI Cells Percent : " << setw(8) << setprecision(2) << fixed
                                      << (100.0 * mcc / cc) << " %" << endl;
}

/**
 * \brief Print blocks distribution between ranks.
 *
 * \param[in] os - stream
 */
void Grid::Print_Blocks_Distribution(ostream &os, int ranks)
{
//    int ranks = Lib::MPI::Ranks_Count();

    os << "Blocks distribution:" << endl;

    /*
     * We need following information:
     * - process number,
     * - blocks count,
     * - blocks percent,
     * - cells count,
     * - cells percent,
     * - interface cells count,
     * - interface cells percent,
     * - shadow cells count,
     * - shadwo cells percent,
     * - MPI cells count,
     * - MPI cells percent.
     */

    /**
     * \brief Class for distribution statistic.
     */
    class Stat
    {
        public:

            // Constructor.
            Stat()
                : Process_Number(0),
                  Blocks_Count(0),
                  Blocks_Percent(0.0),
                  Cells_Count(0),
                  Cells_Percent(0.0),
                  Iface_Cells_Count(0),
                  Iface_Cells_Percent(0.0),
                  Shadow_Cells_Count(0),
                  Shadow_Cells_Percent(0.0),
                  MPI_Cells_Count(0),
                  MPI_Cells_Percent(0.0)
            {}

            // Print.
            void Print(ostream &os)
            {
                os << " | " << setw(8) << Blocks_Count
                   << " | " << setw(6) << setprecision(2) << fixed << Blocks_Percent
                   << " % | " << setw(8) << Cells_Count
                   << " | " << setw(6) << setprecision(2) << fixed << Cells_Percent
                   << " % | " << setw(8) << Iface_Cells_Count
                   << " | " << setw(6) << setprecision(2) << fixed << Iface_Cells_Percent
                   << " % | " << setw(8) << Shadow_Cells_Count
                   << " | " << setw(6) << setprecision(2) << fixed << Shadow_Cells_Percent
                   << " % | " << setw(8) << MPI_Cells_Count
                   << " | " << setw(6) << setprecision(2) << fixed << MPI_Cells_Percent
                   << " % |" << endl;
            }

            // Statistic fields.
            int Process_Number,
                Blocks_Count, Cells_Count, Iface_Cells_Count,
                Shadow_Cells_Count, MPI_Cells_Count;
            double Blocks_Percent, Cells_Percent, Iface_Cells_Percent,
                   Shadow_Cells_Percent, MPI_Cells_Percent;
    };

    // Array of statistic elements.
    Stat *m = new Stat[ranks + 4];

    // Set processec numbers.
    for (int i = 0; i < ranks; i++)
    {
        m[i].Process_Number = i;
    }
    m[ranks + 1].Process_Number = -1;
    m[ranks + 2].Process_Number = -1;

    // Set blocks and cells count.
    for (int i = 0; i < Blocks_Count(); i++)
    {
        Block *p = Get_Block(i);
        int r = p->Rank();
        int cc = p->Cells_Count();
        int icc = p->Iface_Cells_Count();
        int scc = p->Shadow_Cells_Count();

        m[r].Blocks_Count++;
        m[r].Cells_Count += cc;
        m[r].Iface_Cells_Count += icc;
        m[r].Shadow_Cells_Count += scc;

        // total
        m[ranks].Blocks_Count++;
        m[ranks].Cells_Count += cc;
        m[ranks].Iface_Cells_Count += icc;
        m[ranks].Shadow_Cells_Count += scc;
    }

    // Find MPI cells.
    for (int i = 0; i < Ifaces_Count(); i++)
    {
        Iface *p = Get_Iface(i);
        int r = p->B()->Rank();
        int cc = p->Cells_Count() * HYDRO_GRID_SHADOW_DEPTH;

        if (r != p->NB()->Rank())
        {
            m[r].MPI_Cells_Count += cc;
            m[ranks].MPI_Cells_Count += cc;
        }
    }

    // Set min/max.
    if (ranks > 0)
    {
        m[ranks + 1].Blocks_Count = m[0].Blocks_Count;
        m[ranks + 1].Cells_Count = m[0].Cells_Count;
        m[ranks + 1].Iface_Cells_Count = m[0].Iface_Cells_Count;
        m[ranks + 1].Shadow_Cells_Count = m[0].Shadow_Cells_Count;
        m[ranks + 1].MPI_Cells_Count = m[0].MPI_Cells_Count;
        m[ranks + 2].Blocks_Count = m[0].Blocks_Count;
        m[ranks + 2].Cells_Count = m[0].Cells_Count;
        m[ranks + 2].Iface_Cells_Count = m[0].Iface_Cells_Count;
        m[ranks + 2].Shadow_Cells_Count = m[0].Shadow_Cells_Count;
        m[ranks + 2].MPI_Cells_Count = m[0].MPI_Cells_Count;
    }
    for (int i = 1; i < ranks; i++)
    {
        if (m[i].Blocks_Count < m[ranks + 1].Blocks_Count)
        {
            m[ranks + 1].Blocks_Count = m[i].Blocks_Count;
        }
        if (m[i].Cells_Count < m[ranks + 1].Cells_Count)
        {
            m[ranks + 1].Cells_Count = m[i].Cells_Count;
        }
        if (m[i].Iface_Cells_Count < m[ranks + 1].Iface_Cells_Count)
        {
            m[ranks + 1].Iface_Cells_Count = m[i].Iface_Cells_Count;
        }
        if (m[i].Shadow_Cells_Count < m[ranks + 1].Shadow_Cells_Count)
        {
            m[ranks + 1].Shadow_Cells_Count = m[i].Shadow_Cells_Count;
        }
        if (m[i].MPI_Cells_Count < m[ranks + 1].MPI_Cells_Count)
        {
            m[ranks + 1].MPI_Cells_Count = m[i].MPI_Cells_Count;
        }
        if (m[i].Blocks_Count > m[ranks + 2].Blocks_Count)
        {
            m[ranks + 2].Blocks_Count = m[i].Blocks_Count;
        }
        if (m[i].Cells_Count > m[ranks + 2].Cells_Count)
        {
            m[ranks + 2].Cells_Count = m[i].Cells_Count;
        }
        if (m[i].Iface_Cells_Count > m[ranks + 2].Iface_Cells_Count)
        {
            m[ranks + 2].Iface_Cells_Count = m[i].Iface_Cells_Count;
        }
        if (m[i].Shadow_Cells_Count > m[ranks + 2].Shadow_Cells_Count)
        {
            m[ranks + 2].Shadow_Cells_Count = m[i].Shadow_Cells_Count;
        }
        if (m[i].MPI_Cells_Count > m[ranks + 2].MPI_Cells_Count)
        {
            m[ranks + 2].MPI_Cells_Count = m[i].MPI_Cells_Count;
        }
    }

    // Correct percentage.
    for (int i = 0; i < ranks + 3; i++)
    {
        m[i].Blocks_Percent = (m[ranks].Blocks_Count != 0)
                              ? (100.0 * m[i].Blocks_Count / m[ranks].Blocks_Count)
                              : 0.0;
        m[i].Cells_Percent = (m[ranks].Cells_Count != 0)
                             ? (100.0 * m[i].Cells_Count / m[ranks].Cells_Count)
                             : 0.0;
        m[i].Iface_Cells_Percent = (m[ranks].Iface_Cells_Count != 0)
                                    ? (100.0 * m[i].Iface_Cells_Count
                                       / m[ranks].Cells_Count)
                                    : 0.0;
        m[i].Shadow_Cells_Percent = (m[ranks].Shadow_Cells_Count != 0)
                                     ? (100.0 * m[i].Shadow_Cells_Count
                                        / m[ranks].Cells_Count)
                                     : 0.0;
        m[i].MPI_Cells_Percent = (m[ranks].MPI_Cells_Count != 0)
                                 ? (100.0 * m[i].MPI_Cells_Count / m[ranks].Cells_Count)
                                 : 0.0;
    }

    // Set mean.
    m[ranks + 3].Blocks_Count = m[ranks].Blocks_Count / ranks;
    m[ranks + 3].Blocks_Percent = m[ranks].Blocks_Percent / ranks;
    m[ranks + 3].Cells_Count = m[ranks].Cells_Count / ranks;
    m[ranks + 3].Cells_Percent = m[ranks].Cells_Percent / ranks;
    m[ranks + 3].Iface_Cells_Count = m[ranks].Iface_Cells_Count / ranks;
    m[ranks + 3].Iface_Cells_Percent = m[ranks].Iface_Cells_Percent / ranks;
    m[ranks + 3].Shadow_Cells_Count = m[ranks].Shadow_Cells_Count / ranks;
    m[ranks + 3].Shadow_Cells_Percent = m[ranks].Shadow_Cells_Percent / ranks;
    m[ranks + 3].MPI_Cells_Count = m[ranks].MPI_Cells_Count / ranks;
    m[ranks + 3].MPI_Cells_Percent = m[ranks].MPI_Cells_Percent / ranks;

    // Print result.
    os << "*----------*----------*----------*----------*----------*----------*----------*----------*----------*----------*----------*" << endl;
    os << "| Process  | Blocks   | Blocks   | Cells    | Cells    | I. Cells | I. Cells | S. Cells | S. Cells | MPI Cls. | MPI Cls. |" << endl;
    os << "|   Number |    Count |  Percent |    Count |  Percent |    Count |  Percent |    Count |  Percent |    Count |  Percent |" << endl;
    os << "*----------*----------*----------*----------*----------*----------*----------*----------*----------*----------*----------*" << endl;
    for (int i = 0; i < ranks; i++)
    {
        os << "| " << setw(8) << m[i].Process_Number;
        m[i].Print(os);
    }
    os << "*----------*----------*----------*----------*----------*----------*----------*----------*----------*----------*----------*" << endl;
    os << "| " << setw(8) << "total";
    m[ranks].Print(os);
    os << "| " << setw(8) << "min";
    m[ranks + 1].Print(os);
    os << "| " << setw(8) << "max";
    m[ranks + 2].Print(os);
    os << "| " << setw(8) << "mean";
    m[ranks + 3].Print(os);
    os << "*----------*----------*----------*----------*----------*----------*----------*----------*----------*----------*----------*" << endl;

    // Free memory.
    delete m;
}

} }

