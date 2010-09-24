// -*- C++ -*-
//
// ======================================================================
//
// Brad T. Aagaard, U.S. Geological Survey
// Charles A. Williams, GNS Science
// Matthew G. Knepley, University of Chicago
//
// This code was developed as part of the Computational Infrastructure
// for Geodynamics (http://geodynamics.org).
//
// Copyright (c) 2010 University of California, Davis
//
// See COPYING for license information.
//
// ======================================================================
//

#include <portinfo>

#include "MeshRefiner.hh" // implementation of class methods

#include "CellRefinerTri3.hh" // USES CellRefinerTri3
#include "MeshOrder.hh" // USES MeshOrder

#include <cassert> // USES assert()

// ----------------------------------------------------------------------
// Constructor
ALE::MeshRefiner::MeshRefiner(void) :
  _orderOldMesh(new MeshOrder),
  _orderNewMesh(new MeshOrder)
{ // constructor
} // constructor

// ----------------------------------------------------------------------
// Destructor
ALE::MeshRefiner::~MeshRefiner(void)
{ // destructor
  delete _orderOldMesh; _orderOldMesh = PETSC_NULL;
  delete _orderNewMesh; _orderNewMesh = PETSC_NULL;
} // destructor

// ----------------------------------------------------------------------
// Refine mesh.
void
ALE::MeshRefiner::refine(const Obj<mesh_type>& newMesh, 
			 const Obj<mesh_type>& mesh, 
			 CellRefinerTri3& refiner)
{ // refine
  assert(!mesh.isNull());
  if (mesh->hasLabel("censored depth")) {
    _refineCensored(newMesh, mesh, refiner);
  } else {
    _refine(newMesh, mesh, refiner);
  } // if/else
} // refine

// ----------------------------------------------------------------------
// Refine mesh.
void
ALE::MeshRefiner::_refine(const Obj<mesh_type>& newMesh, 
			  const Obj<mesh_type>& mesh, 
			  CellRefinerTri3& refiner)
{ // _refine
  typedef Interval<point_type>::const_iterator interval_type;

  assert(_orderOldMesh);
  assert(_orderNewMesh);

  // Calculate order in old mesh.
  _orderOldMesh->initialize(mesh);

  assert(!mesh.isNull());
  assert(!newMesh.isNull());
  
  // Get original mesh stuff.
  const Obj<mesh_type::label_sequence>& cells = mesh->heightStratum(0);
  assert(!cells.isNull());
  const mesh_type::label_sequence::iterator cellsEnd = cells->end();
  
  const Obj<mesh_type::label_sequence>& vertices = mesh->depthStratum(0);
  assert(!vertices.isNull());
  
  const Obj<mesh_type::sieve_type>& sieve = mesh->getSieve();
  assert(!sieve.isNull());
  ALE::ISieveVisitor::PointRetriever<mesh_type::sieve_type> cV(std::max(1, sieve->getMaxConeSize()));
  
  // Count number of cells.
  const int oldNumCells = cells->size();
  int newNumCells = 0;
  for (mesh_type::label_sequence::iterator c_iter = cells->begin(); c_iter != cellsEnd; ++c_iter) {
    newNumCells += refiner.numNewCells(*c_iter);
  } // for

  // Count number of vertices.
  const int oldNumVertices = vertices->size();
  int counterBegin = newNumCells + oldNumVertices;
  point_type curNewVertex = counterBegin;
  for(mesh_type::label_sequence::iterator c_iter = cells->begin(); c_iter != cellsEnd; ++c_iter) {
    cV.clear();
    sieve->cone(*c_iter, cV);
    refiner.splitCell(*c_iter, cV.getPoints(), cV.getSize(), &curNewVertex);
  } // for
  const int newNumVertices = oldNumVertices + curNewVertex - counterBegin;

  _orderNewMesh->cellsNormal(0, newNumCells);
  _orderNewMesh->verticesNormal(newNumCells, newNumCells+newNumVertices);
  _orderNewMesh->verticesCensored(newNumCells+newNumVertices, newNumCells+newNumVertices);
  _orderNewMesh->cellsCensored(newNumCells+newNumVertices, newNumCells+newNumVertices);
  
  // Allocate chart for new sieve.
  const Obj<mesh_type::sieve_type>& newSieve = newMesh->getSieve();
  assert(!newSieve.isNull());
  newSieve->setChart(mesh_type::sieve_type::chart_type(0, _orderNewMesh->cellsCensored().max()));

  // Create new sieve with correct sizes for refined cells
  point_type curNewCell = _orderNewMesh->cellsNormal().min();
  const interval_type::const_iterator oldCellsEnd = _orderOldMesh->cellsNormal().end();
  for (interval_type::const_iterator c_iter=_orderOldMesh->cellsNormal().begin(); c_iter != oldCellsEnd; ++c_iter) {
    // Set new cone and support sizes
    cV.clear();
    sieve->cone(*c_iter, cV);
    const point_type* cone = cV.getPoints();
    const int coneSize = cV.getSize();

    const point_type* newCells;
    int numNewCells = 0;
    refiner.getNewCells(&newCells, &numNewCells, *c_iter, cone, coneSize, *_orderOldMesh, *_orderNewMesh);

    for(int iCell=0; iCell < numNewCells; ++iCell, ++curNewCell) {
      newSieve->setConeSize(curNewCell, coneSize);
      for(int iVertex=0; iVertex < coneSize; ++iVertex) {
	newSieve->addSupportSize(newCells[iCell*coneSize+iVertex], 1);
      } // for
    } // for

  } // for
  newSieve->allocate();

  // Create refined cells in new sieve.
  curNewCell = _orderNewMesh->cellsNormal().min();
  for (interval_type::const_iterator c_iter=_orderOldMesh->cellsNormal().begin(); c_iter != oldCellsEnd; ++c_iter) {
    cV.clear();
    sieve->cone(*c_iter, cV);
    const point_type *cone = cV.getPoints();
    const int coneSize = cV.getSize();
    
    const point_type* newCells;
    int numNewCells = 0;
    refiner.getNewCells(&newCells, &numNewCells, *c_iter, cone, coneSize, *_orderOldMesh, *_orderNewMesh);
    
    for(int iCell=0; iCell < numNewCells; ++iCell, ++curNewCell) {
      newSieve->setCone(&newCells[iCell*coneSize], curNewCell);
    } // for
  } // for
  newSieve->symmetrize();

  // Set coordinates in refined mesh.
  const Obj<mesh_type::real_section_type>& coordinates = mesh->getRealSection("coordinates");
  assert(!coordinates.isNull());
  const Obj<mesh_type::real_section_type>& newCoordinates = newMesh->getRealSection("coordinates");
  assert(!newCoordinates.isNull());

  const mesh_type::label_sequence::const_iterator verticesEnd = vertices->end();
  assert(vertices->size() > 0);
  const int spaceDim = coordinates->getFiberDimension(*vertices->begin());
  assert(spaceDim > 0);
  newCoordinates->setChart(mesh_type::sieve_type::chart_type(_orderNewMesh->verticesNormal().min(), _orderNewMesh->verticesCensored().max()));

  interval_type::const_iterator newVerticesEnd = _orderNewMesh->verticesCensored().end();
  for (interval_type::const_iterator v_iter=_orderNewMesh->verticesNormal().begin(); v_iter != newVerticesEnd; ++v_iter) {
    newCoordinates->setFiberDimension(*v_iter, spaceDim);
  } // for
  newCoordinates->allocatePoint();
      
  const interval_type::const_iterator oldVerticesEnd = _orderOldMesh->verticesCensored().end();
  for (interval_type::const_iterator vOld_iter=_orderOldMesh->verticesNormal().begin(), vNew_iter=_orderNewMesh->verticesNormal().begin(); vOld_iter != oldVerticesEnd; ++vOld_iter, ++vNew_iter) {
    std::cout << "Copy coordinates from old vertex " << *vOld_iter << " to new vertex " << *vNew_iter << std::endl;
    newCoordinates->updatePoint(*vNew_iter, coordinates->restrictPoint(*vOld_iter));
  } // for

  refiner.setCoordsNewVertices(newCoordinates, coordinates);

  _stratify(newMesh);
  _calcNewOverlap(newMesh, mesh);
} // _refine
  
// ----------------------------------------------------------------------
// Refine mesh with a censored depth.
  void
    ALE::MeshRefiner::_refineCensored(const Obj<mesh_type>& newMesh, 
				      const Obj<mesh_type>& mesh, 
				      CellRefinerTri3& refiner)
{ // _refineCensored
} // _refineCensored

// ----------------------------------------------------------------------
// Stratify mesh.
void
ALE::MeshRefiner::_stratify(const Obj<mesh_type>& mesh)
{ // _stratify
  typedef Interval<point_type>::const_iterator interval_type;

  assert(_orderNewMesh);
  assert(_orderOldMesh);
	 
  // Fast stratification
  const Obj<mesh_type::label_type>& height = mesh->createLabel("height");
  const Obj<mesh_type::label_type>& depth  = mesh->createLabel("depth");

  // Set height/depth of cells
  interval_type::const_iterator cellsEnd = _orderNewMesh->cellsNormal().end();
  for (interval_type::const_iterator c_iter = _orderNewMesh->cellsNormal().begin(); c_iter != cellsEnd; ++c_iter) {
    height->setCone(0, *c_iter);
    depth->setCone(1, *c_iter);
  } // for
  cellsEnd = _orderNewMesh->cellsCensored().end();
  for (interval_type::const_iterator c_iter = _orderNewMesh->cellsCensored().begin(); c_iter != cellsEnd; ++c_iter) {
    height->setCone(0, *c_iter);
    depth->setCone(1, *c_iter);
  } // for

  // Set height/depth of vertices
  interval_type::const_iterator verticesEnd = _orderNewMesh->verticesNormal().end();
  for (interval_type::const_iterator v_iter = _orderNewMesh->verticesNormal().begin(); v_iter != verticesEnd; ++v_iter) {
    height->setCone(1, *v_iter);
    depth->setCone(0, *v_iter);
  } // for
  verticesEnd = _orderNewMesh->verticesCensored().end();
  for (interval_type::const_iterator v_iter = _orderNewMesh->verticesCensored().begin(); v_iter != verticesEnd; ++v_iter) {
    height->setCone(1, *v_iter);
    depth->setCone(0, *v_iter);
  } // for
  
  mesh->setHeight(1);
  mesh->setDepth(1);
} // _stratify

// ----------------------------------------------------------------------
// Calculate new overlap.
void
ALE::MeshRefiner::_calcNewOverlap(const Obj<mesh_type>& newMesh,
				  const Obj<mesh_type>& mesh)
{ // _calcNewOverlap
  // Exchange new boundary vertices
  //   We can convert endpoints, and then just match to new vertex on this side
  //   1) Create the overlap of edges which are vertex pairs (do not need for interpolated meshes)
  //   2) Create a section of overlap edge --> new vertex (this will generalize to other split points in interpolated meshes)
  //   3) Copy across new overlap
  //   4) Fuse matches new vertex pairs and inserts them into the old overlap

  // Create the parallel overlap
#if 0
  int *oldNumCellsP    = new int[mesh->commSize()];
  int *newNumCellsP = new int[newMesh->commSize()];
  int  ierr;
  
  ierr = MPI_Allgather((void *) &oldNumCells, 1, MPI_INT, oldNumCellsP, 1, MPI_INT, mesh->comm());CHKERRXX(ierr);
  ierr = MPI_Allgather((void *) &newNumCells, 1, MPI_INT, newNumCellsP, 1, MPI_INT, newMesh->comm());CHKERRXX(ierr);
  Obj<mesh_type::send_overlap_type> newSendOverlap = newMesh->getSendOverlap();
  Obj<mesh_type::recv_overlap_type> newRecvOverlap = newMesh->getRecvOverlap();
  const Obj<mesh_type::send_overlap_type>& sendOverlap = mesh->getSendOverlap();
  const Obj<mesh_type::recv_overlap_type>& recvOverlap = mesh->getRecvOverlap();
  Obj<mesh_type::send_overlap_type::traits::capSequence> sendPoints  = sendOverlap->cap();
  const mesh_type::send_overlap_type::source_type        localOffset = newNumCellsP[newMesh->commRank()] - oldNumCellsP[mesh->commRank()];
  
  for(mesh_type::send_overlap_type::traits::capSequence::iterator p_iter = sendPoints->begin(); p_iter != sendPoints->end(); ++p_iter) {
    const Obj<mesh_type::send_overlap_type::traits::supportSequence>& ranks      = sendOverlap->support(*p_iter);
    const mesh_type::send_overlap_type::source_type&                  localPoint = *p_iter;
    
    for(mesh_type::send_overlap_type::traits::supportSequence::iterator r_iter = ranks->begin(); r_iter != ranks->end(); ++r_iter) {
      const int                                   rank         = *r_iter;
      const mesh_type::send_overlap_type::source_type& remotePoint  = r_iter.color();
      const mesh_type::send_overlap_type::source_type  remoteOffset = newNumCellsP[rank] - oldNumCellsP[rank];
      
      newSendOverlap->addArrow(localPoint+localOffset, rank, remotePoint+remoteOffset);
    }
  }
  Obj<mesh_type::recv_overlap_type::traits::baseSequence> recvPoints = recvOverlap->base();
  
  for(mesh_type::recv_overlap_type::traits::baseSequence::iterator p_iter = recvPoints->begin(); p_iter != recvPoints->end(); ++p_iter) {
    const Obj<mesh_type::recv_overlap_type::traits::coneSequence>& ranks      = recvOverlap->cone(*p_iter);
    const mesh_type::recv_overlap_type::target_type&               localPoint = *p_iter;
    
    for(mesh_type::recv_overlap_type::traits::coneSequence::iterator r_iter = ranks->begin(); r_iter != ranks->end(); ++r_iter) {
      const int                                        rank         = *r_iter;
      const mesh_type::recv_overlap_type::target_type& remotePoint  = r_iter.color();
      const mesh_type::recv_overlap_type::target_type  remoteOffset = newNumCellsP[rank] - oldNumCellsP[rank];
      
      newRecvOverlap->addArrow(rank, localPoint+localOffset, remotePoint+remoteOffset);
    }
  }
  newMesh->setCalculatedOverlap(true);
  delete [] oldNumCellsP;
  delete [] newNumCellsP;
  // Check edges in edge2vertex for both endpoints sent to same process
  //   Put it in section with point being the lowest numbered vertex and value (other endpoint, new vertex)
  Obj<ALE::Section<point_type, edge_type> > newVerticesSection = new ALE::Section<point_type, edge_type>(mesh->comm());
  std::map<edge_type, std::vector<int> > bdedge2rank;
  
  for(std::map<edge_type, point_type>::const_iterator e_iter = edge2vertex.begin(); e_iter != edge2vertex.end(); ++e_iter) {
    const point_type left  = e_iter->first.first;
    const point_type right = e_iter->first.second;
    
    if (sendOverlap->capContains(left) && sendOverlap->capContains(right)) {
      const Obj<mesh_type::send_overlap_type::traits::supportSequence>& leftRanksSeq = sendOverlap->support(left);
      std::list<int> leftRanks(leftRanksSeq->begin(), leftRanksSeq->end());
      const Obj<mesh_type::send_overlap_type::traits::supportSequence>& rightRanks   = sendOverlap->support(right);
      std::list<int> ranks;
      std::set_intersection(leftRanks.begin(), leftRanks.end(), rightRanks->begin(), rightRanks->end(),
			    std::insert_iterator<std::list<int> >(ranks, ranks.begin()));
      
      if(ranks.size()) {
	newVerticesSection->addFiberDimension(std::min(e_iter->first.first, e_iter->first.second)+localOffset, 1);
	for(std::list<int>::const_iterator r_iter = ranks.begin(); r_iter != ranks.end(); ++r_iter) {
	  bdedge2rank[e_iter->first].push_back(*r_iter);
	}
      }
    }
  }
  newVerticesSection->allocatePoint();
  const ALE::Section<point_type, edge_type>::chart_type& chart = newVerticesSection->getChart();
  
  for(ALE::Section<point_type, edge_type>::chart_type::const_iterator c_iter = chart.begin(); c_iter != chart.end(); ++c_iter) {
    typedef ALE::Section<point_type, edge_type>::value_type value_type;
    const point_type p      = *c_iter;
    const int        dim    = newVerticesSection->getFiberDimension(p);
    int              v      = 0;
    value_type      *values = new value_type[dim];
    
    for(std::map<edge_type, std::vector<int> >::const_iterator e_iter = bdedge2rank.begin(); e_iter != bdedge2rank.end() && v < dim; ++e_iter) {
      if (std::min(e_iter->first.first, e_iter->first.second)+localOffset == p) {
	values[v++] = edge_type(std::max(e_iter->first.first, e_iter->first.second)+localOffset, edge2vertex[e_iter->first]);
      }
    }
    newVerticesSection->updatePoint(p, values);
    delete [] values;
  }
  // Copy across overlap
  typedef ALE::Pair<int, point_type> overlap_point_type;
  Obj<ALE::Section<overlap_point_type, edge_type> > overlapVertices = new ALE::Section<overlap_point_type, edge_type>(mesh->comm());
  
  ALE::Pullback::SimpleCopy::copy(newSendOverlap, newRecvOverlap, newVerticesSection, overlapVertices);
  // Merge by translating edge to local points, finding edge in edge2vertex, and adding (local new vetex, remote new vertex) to overlap
  for(std::map<edge_type, std::vector<int> >::const_iterator e_iter = bdedge2rank.begin(); e_iter != bdedge2rank.end(); ++e_iter) {
    const point_type localPoint = edge2vertex[e_iter->first];
    
    for(std::vector<int>::const_iterator r_iter = e_iter->second.begin(); r_iter != e_iter->second.end(); ++r_iter) {
      point_type remoteLeft = -1, remoteRight = -1;
      const int  rank       = *r_iter;
      
      const Obj<mesh_type::send_overlap_type::traits::supportSequence>& leftRanks = newSendOverlap->support(e_iter->first.first+localOffset);
      for(mesh_type::send_overlap_type::traits::supportSequence::iterator lr_iter = leftRanks->begin(); lr_iter != leftRanks->end(); ++lr_iter) {
	if (rank == *lr_iter) {
	  remoteLeft = lr_iter.color();
	  break;
	}
      }
      const Obj<mesh_type::send_overlap_type::traits::supportSequence>& rightRanks = newSendOverlap->support(e_iter->first.second+localOffset);
      for(mesh_type::send_overlap_type::traits::supportSequence::iterator rr_iter = rightRanks->begin(); rr_iter != rightRanks->end(); ++rr_iter) {
	if (rank == *rr_iter) {
	  remoteRight = rr_iter.color();
	  break;
	}
      }
      const point_type remoteMin   = std::min(remoteLeft, remoteRight);
      const point_type remoteMax   = std::max(remoteLeft, remoteRight);
      const int        remoteSize  = overlapVertices->getFiberDimension(overlap_point_type(rank, remoteMin));
      const edge_type *remoteVals  = overlapVertices->restrictPoint(overlap_point_type(rank, remoteMin));
      point_type       remotePoint = -1;
      
      for(int d = 0; d < remoteSize; ++d) {
	if (remoteVals[d].first == remoteMax) {
	  remotePoint = remoteVals[d].second;
	  break;
	}
      }
      newSendOverlap->addArrow(localPoint, rank, remotePoint);
      newRecvOverlap->addArrow(rank, localPoint, remotePoint);
    }
  }
#endif
} // _calcNewOverlap


// End of file 