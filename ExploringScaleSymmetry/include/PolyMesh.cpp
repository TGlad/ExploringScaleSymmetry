// Thomas Lowe, 2020.
// Polygon mesh class that supports mirroring a mesh around a plane (splitting some triangles in the process). 
#include "PolyMesh.h"
#include <iostream>
#include <fstream>

void PolyMesh::reflectFace(int faceID, const Vector3d &pos, const Vector3d &normal)
{
  const double eps = 1e-8;
  Face &my = faces[faceID];
  Face::FaceNode *f = my.head;
  Face::FaceNode *split[2] = { NULL, NULL };
  double d = (nodes[my.head->nodeID].pos - pos).dot(normal);
  double dir = d > 0 ? 1 : -1; // TODO: check 
  int s = 0;
  do
  {
    int n1 = f->nodeID;
    int n2 = f->next->nodeID;
    double d = (nodes[n2].pos - pos).dot(normal);
    if (d * dir < eps) // gonna split
    {
      if (d * dir < -eps) // gonna need a new node down an edge
      {
        int n = min(n1, n2);
        int nodeAddedJ = -1;
        for (int j = 0; j<(int)nodes[n].newNodesAdded.size(); j++)
          if (nodes[n].newNodesAdded[j][0] == max(n1, n2))
            nodeAddedJ = j;

        int nodeAdded;
        if (nodeAddedJ == -1) // add a new node
        {
          nodeAdded = nodes.size();
          nodes[n].newNodesAdded.push_back(Vector2i(max(n1, n2), nodeAdded));
          Vertex newNode;
          double t = d / (nodes[n2].pos - nodes[n1].pos).dot(normal);
          newNode.pos = nodes[n2].pos + (nodes[n1].pos - nodes[n2].pos)*t;
          newNode.uv = nodes[n2].uv + (nodes[n1].uv - nodes[n2].uv)*t;
          nodes.push_back(newNode);
        }
        else
          nodeAdded = nodes[n].newNodesAdded[nodeAddedJ][1];
        split[s] = new Face::FaceNode;
        split[s]->next = f->next;
        split[s]->nodeID = nodeAdded;
        f->next = split[s];
      }
      else  // split on a vertex
      {
        // but we should only split if the following node is definitely over the line...
        double d2 = (nodes[f->next->next->nodeID].pos - pos).dot(normal);
        if (d2 * dir < -eps) // gonna split
          split[s] = f->next;
        else
        {
          f = f->next;
          continue;
        }
      }
      if (s == 1) // connect to make a new face and reconnect the other face linked list
      {
        Face newFace;
        newFace.head = new Face::FaceNode;
        newFace.head->nodeID = split[0]->nodeID;
        newFace.head->next = split[0]->next;
        faces.push_back(newFace);
        split[0]->next = split[1];

        Face::FaceNode *new1 = new Face::FaceNode;
        new1->nodeID = split[1]->nodeID;
        new1->next = newFace.head;

        f->next = new1;
        break;
      }
      dir *= -1;
      s++;
    }
    f = f->next;
  } while (f != my.head);
}

void PolyMesh::reflectNode(int i, const Vector3d &pos, const Vector3d &normal)
{
  if ((nodes[i].pos - pos).dot(normal) < 0)
  {
    didReflect = true;
    nodes[i].pos -= normal * 2.0*(nodes[i].pos - pos).dot(normal);
  }
}

bool PolyMesh::reflect(const Vector3d &pos, const Vector3d &normal)
{
  didReflect = false;
  for (int i = 0; i<(int)nodes.size(); i++)
    nodes[i].newNodesAdded.clear();
  int numFaces = faces.size();
  for (int i = 0; i<numFaces; i++)
    reflectFace(i, pos, normal);
  for (int i = 0; i < (int)nodes.size(); i++)
    reflectNode(i, pos, normal);
  return didReflect;
}

PolyMesh::PolyMesh()
{
  didReflect = false;
  Vertex n1;
  Vertex n2(Vector3d(0, 0, 0), Vector2d(0, 0));
  // initialise to a square, flat bit of paper
  nodes.resize(4);
  nodes[0] = Vertex(Vector3d(0, 0, 0), Vector2d(0, 0));
  nodes[1] = Vertex(Vector3d(1, 0, 0), Vector2d(1, 0));
  nodes[2] = Vertex(Vector3d(1, 1, 0), Vector2d(1, 1));
  nodes[3] = Vertex(Vector3d(0, 1, 0), Vector2d(0, 1));
  faces.resize(1);

  faces[0].head = new Face::FaceNode;
  Face::FaceNode *fn = faces[0].head;
  faces[0].head->nodeID = 0;
  for (int i = 1; i < 4; i++)
  {
    Face::FaceNode *newN = new Face::FaceNode;
    newN->nodeID = i;
    fn->next = newN;
    fn = fn->next;
  }
  fn->next = faces[0].head;
}

void PolyMesh::savePLY(const string &fileName, const Vector3d &offset)
{
  cout << "saving to file " << fileName << endl;
  ofstream myfile;
#define ASCII_SAVE
#if defined(ASCII_SAVE)
  myfile.open(fileName.c_str());
#else
  myfile.open(fileName.c_str(), std::ios_base::binary);
#endif
  myfile << "ply\n";
#if defined(ASCII_SAVE)
  myfile << "format ascii 1.0\n";
#else
  myfile << "format binary_little_endian 1.0\n";
#endif
  myfile << "element vertex " << nodes.size() << "\n";
  myfile << "property float x\n";
  myfile << "property float y\n";
  myfile << "property float z\n";
  myfile << "element face " << faces.size() << "\n";
  myfile << "property list int int vertex_index\n";
  myfile << "end_header\n";
#if defined(ASCII_SAVE)
  for (int i = 0; i < (int)nodes.size(); i++)
    myfile << nodes[i].pos[0]+offset[0] << " " << nodes[i].pos[1]+offset[1] << " " << nodes[i].pos[2]+offset[2] << "\n";
#else
  vector<Vector3f> vertices(nodes.size()); // 3d to give space for colour
  for (unsigned int i = 0; i<nodes.size(); i++)
    vertices[i] << (float)nodes[i].pos[0]+offset[0], (float)nodes[i].pos[1]+offset[1], (float)nodes[i].pos[2]+offset[2];
  myfile.write((char *)&vertices[0], sizeof(Vector3f)*vertices.size());
#endif
#if defined(ASCII_SAVE)
  for (int i = 0; i < (int)faces.size(); i++)
  {
    int n = 1;
    Face::FaceNode *fn = faces[i].head;
    while (fn->next != faces[i].head)
    {
      n++;
      fn = fn->next;
    }
    myfile << n << " ";
    fn = faces[i].head;
    do
    {
      myfile << fn->nodeID << " ";
      fn = fn->next;
    } while (fn != faces[i].head);
    myfile << "\n";
  }
#else
  vector<int> facelist; 
  vector<float> uvlist; 
  for (int i = 0; i < (int)faces.size(); i++)
  {
    int n = 1;
    Face::FaceNode *fn = faces[i].head;
    while (fn->next != faces[i].head)
    {
      n++;
      fn = fn->next;
    }
    facelist.push_back(n);
    fn = faces[i].head;
    do
    {
      facelist.push_back(fn->nodeID);
      fn = fn->next;
    } while (fn != faces[i].head);
  }
  myfile.write((char *)&facelist[0], sizeof(int)*facelist.size());
#endif
  myfile.close();
  cout << "finished saving." << endl;
}

void PolyMesh::saveOBJ(const string &fileName, const Vector3d &offset)
{
  cout << "saving to file " << fileName << endl;
  ofstream myfile;
  myfile.open(fileName.c_str());
  for (int i = 0; i < (int)nodes.size(); i++)
    myfile << "v " << nodes[i].pos[0] + offset[0] << " " << nodes[i].pos[1] + offset[1] << " " << nodes[i].pos[2] + offset[2] << "\n";
  for (int i = 0; i < (int)nodes.size(); i++)
    myfile << "vt " << nodes[i].uv[0] << " " << nodes[i].uv[1] << "\n";
  for (int i = 0; i < (int)faces.size(); i++)
  {
    int n = 1;
    Face::FaceNode *fn = faces[i].head;
    while (fn->next != faces[i].head)
    {
      n++;
      fn = fn->next;
    }
    myfile << "f ";
    fn = faces[i].head;
    do
    {
      myfile << fn->nodeID+1 << "/" << fn->nodeID+1 << " ";
      fn = fn->next;
    } while (fn != faces[i].head);
    myfile << "\n";
  }
  myfile.close();
  cout << "finished saving." << endl;
}

static ofstream svg;
static double scale = 900.0;
void PolyMesh::openSVG(const string &fileName, int number)
{
  svg.open(fileName.c_str());

  svg << "<svg width = \"" << (int)(scale * (double)number*1.05) << "\" height = \"" << (int)scale << "\" xmlns = \"http://www.w3.org/2000/svg\">" << endl;
}

void PolyMesh::saveSVG(const Vector3d &offset, double shade)
{
  for (int i = 0; i < (int)faces.size(); i++)
  {
    svg << "<path d = \"M " << scale*(nodes[faces[i].head->nodeID].uv[0] + offset[0]) << " " << scale*(nodes[faces[i].head->nodeID].uv[1] + offset[1]);
    Face::FaceNode *fn = faces[i].head->next;
    do
    {
      svg << " L " << scale*(nodes[fn->nodeID].uv[0] + offset[0]) << " " << scale*(nodes[fn->nodeID].uv[1] + offset[1]);
      fn = fn->next;
    } while (fn != faces[i].head->next);
    svg << "\" fill=\"transparent\" stroke=\"black\" style=\"stroke-opacity:" << shade << "\" />\n";
  }
}

void PolyMesh::closeSVG()
{
  svg << "</svg>" << endl;
  svg.close();
}

void TriMesh::saveOBJ(const string &fileName)
{
  cout << "saving to file " << fileName << endl;
  ofstream myfile;
  myfile.open(fileName.c_str());
  for (int i = 0; i < (int)triangles.size(); i++)
    for (int j = 0; j < 3; j++)
      myfile << "v " << triangles[i].corner[j][0] << " " << triangles[i].corner[j][1] << " " << triangles[i].corner[j][2] << "\n";
  for (int i = 0; i < (int)triangles.size(); i++)
    for (int j = 0; j < 3; j++)
      myfile << "vt " << 0 << " " << 0 << "\n";
  for (int i = 0; i < (int)triangles.size(); i++)
    myfile << "f " << i * 3 << "/" << i * 3 << " " << i * 3 + 1 << "/" << i * 3 + 1 << " " << i * 3 + 2 << "/" << i * 3 + 2 << "\n";
  myfile.close();
  cout << "finished saving." << endl;
}

void TriMesh::savePLY(const string &fileName)
{
  cout << "saving to file " << fileName << endl;
  ofstream myfile;
//#define ASCII_SAVE2
#if defined(ASCII_SAVE2)
  myfile.open(fileName.c_str());
#else
  myfile.open(fileName.c_str(), std::ios_base::binary);
#endif
  myfile << "ply\n";
#if defined(ASCII_SAVE2)
  myfile << "format ascii 1.0\n";
#else
  myfile << "format binary_little_endian 1.0\n";
#endif
  myfile << "element vertex " << triangles.size()*3 << "\n";
  myfile << "property float x\n";
  myfile << "property float y\n";
  myfile << "property float z\n";
  myfile << "element face " << triangles.size() << "\n";
  myfile << "property list int int vertex_index\n";
  myfile << "end_header\n";
#if defined(ASCII_SAVE2)
  for (int i = 0; i < (int)triangles.size(); i++)
    for (int j = 0; j < 3; j++)
      myfile << triangles[i].corner[j][0] << " " << triangles[i].corner[j][1] << " " << triangles[i].corner[j][2] << "\n";
#else
  vector<Vector3f> vertices(triangles.size()*3); // 3d to give space for colour
  for (int i = 0; i < (int)triangles.size(); i++)
    for (int j = 0; j < 3; j++)
      vertices[3*i + j] = Vector3f((float)triangles[i].corner[j][0], (float)triangles[i].corner[j][1], (float)triangles[i].corner[j][2]);
  myfile.write((char *)&vertices[0], sizeof(Vector3f)*vertices.size());
#endif
#if defined(ASCII_SAVE2)
  for (int i = 0; i < (int)triangles.size(); i++)
  {
    myfile << 3 << " " << 3*i << " " << 3*i+1 << " " << 3*i+2 << "\n";
  }
#else
  vector<int> facelist;
  for (int i = 0; i < (int)triangles.size(); i++)
  {
    facelist.push_back(3);
    for (int j = 0; j<3; j++)
      facelist.push_back(3*i+j);
  }
  myfile.write((char *)&facelist[0], sizeof(int)*facelist.size());
#endif
  myfile.close();
  cout << "finished saving." << endl;
}
