#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

class Graph
{
private:
    unordered_map<int, vector<int>> adj;
    int edges, vertices;

public:
    Graph()
    {
        adj.clear();
        edges = vertices = 0;
    }

    void input()
    {
        cout << "Enter number of vertices: ";
        cin >> vertices;
        cout << "Enter number of edges: ";
        cin >> edges;

        for (int i = 0; i < vertices; i++)
        {
            adj[i] = {};
        }

        for (int i = 0; i < edges; i++)
        {
            int src, dst;
            cout << "Enter source vertex No. (0 - " << vertices - 1 << " ): ";
            cin >> src;
            cout << "Enter destination vertex No. (0 - " << vertices - 1 << " ): ";
            cin >> dst;
            adj[src].push_back(dst);
            adj[dst].push_back(src);
            cout << endl;
        }
    }

    void display()
    {
        cout << "Adjacency list:\n";
        for (int i = 0; i < vertices; i++)
        {
            cout << i << " => ";
            for (auto it : adj[i])
            {
                cout << it << " ";
            }
            cout << endl;
        }
    }

    void DFSHelper()
    {
        vector<bool> visited(vertices, false);
        cout << "Enter starting vertex for sequential DFS: ";
        int startVertex;
        cin >> startVertex;
        visited[startVertex] = true;

        cout << startVertex << " ";

        for (auto it : adj[startVertex])
        {
            if (!visited[it])
            {
                visited[it] = true;
                DFS(it, visited);
            }
        }
        cout << endl;
    }

    void ParallelDFSHelper()
    {
        vector<bool> visited(vertices, false);
        cout << "Enter starting vertex for parallel DFS: ";
        int startVertex;
        cin >> startVertex;
        visited[startVertex] = true;

        cout << startVertex << " ";

#pragma omp parallel for
        for (int i = 0; i < adj[startVertex].size(); i++)
        {
            int it = adj[startVertex][i];

#pragma omp critical
            {
                if (!visited[it])
                {
                    visited[it] = true;
                    DFS(it, visited);
                }
            }
        }
        cout << endl;
    }

    void DFS(int it, vector<bool> &visited)
    {
        visited[it] = true;
        cout << it << " ";

        for (int i = 0; i < adj[it].size(); i++)
        {
            int node = adj[it][i];
            if (!visited[node])
            {
                DFS(node, visited);
            }
        }
    }

    void BFS()
    {
        int startVertex;
        cout << "Enter starting vertex for BFS: ";
        cin >> startVertex;

        vector<bool> visited(vertices, false);
        queue<int> q;
        q.push(startVertex);

        while (!q.empty())
        {
            int node = q.front();
            q.pop();

            if (visited[node])
                continue;

            visited[node] = true;
            cout << node << " ";

            for (int i = 0; i < adj[node].size(); i++)
            {
                int it = adj[node][i];
                if (!visited[it])
                {
                    q.push(it);
                }
            }
        }
        cout << endl;
    }

    void ParallelBFS()
    {
        int startVertex;
        cout << "Enter starting vertex for parallel BFS: ";
        cin >> startVertex;

        vector<bool> visited(vertices, false);
        queue<int> q;
        q.push(startVertex);

        while (!q.empty())
        {
            int node;
// Critical block to safely access shared queue
#pragma omp critical
            {
                node = q.front();
                q.pop();
            }

            bool alreadyVisited;
#pragma omp critical
            {
                alreadyVisited = visited[node];
                if (!visited[node])
                {
                    visited[node] = true;
                    cout << node << " ";
                }
            }

            if (alreadyVisited)
                continue;

// Parallelize neighbor traversal
#pragma omp parallel for
            for (int i = 0; i < adj[node].size(); i++)
            {
                int it = adj[node][i];
#pragma omp critical
                {
                    if (!visited[it])
                        q.push(it);
                }
            }
        }
        cout << endl;
    }
    void input2(){
        cout<<"Enter no. of Vertices: ";
        cin>>vertices;
        for(int i=1;i<=vertices;i++){
            for(int mul=i;mul<=vertices;mul+=i){
                if(i==mul){
                    continue;
                }
                adj[i].push_back(mul);
                adj[mul].push_back(i);
            }
            // adj[i]=;
        }

    }
};

int main()
{
    Graph g;
    g.input2();
    g.display();

    double start_time, end_time;

    start_time = omp_get_wtime();
    g.DFSHelper();
    end_time = omp_get_wtime();
    cout << "Time taken by Sequential DFS is " << end_time - start_time << " seconds" << endl;

    start_time = omp_get_wtime();
    g.ParallelDFSHelper();
    end_time = omp_get_wtime();
    cout << "Time taken by Parallel DFS is " << end_time - start_time << " seconds" << endl;

    start_time = omp_get_wtime();
    g.BFS();
    end_time = omp_get_wtime();
    cout << "Time taken by Sequential BFS is " << end_time - start_time << " seconds" << endl;

    start_time = omp_get_wtime();
    g.ParallelBFS();
    end_time = omp_get_wtime();
    cout << "Time taken by Parallel BFS is " << end_time - start_time << " seconds" << endl;

    return 0;
}
