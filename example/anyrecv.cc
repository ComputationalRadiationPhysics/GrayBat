// STL
#include <iostream>   /* std::cout */
#include <vector>     /* std::vector */
#include <array>      /* std::array */
#include <functional> /* std::bind */

// GRAYBAT
#include <graybat/Cage.hpp>
#include <graybat/communicationPolicy/BMPI.hpp>
#include <graybat/graphPolicy/BGL.hpp>
// GRAYBAT mappings
#include <graybat/mapping/Consecutive.hpp>
#include <graybat/mapping/Random.hpp>
#include <graybat/mapping/Roundrobin.hpp>
// GRAYBAT pattern
#include <graybat/pattern/GridDiagonal.hpp>
#include <graybat/pattern/Chain.hpp>
#include <graybat/pattern/Star.hpp>
#include <graybat/pattern/BidirectionalStar.hpp>


struct Function {
    
    void process(std::array<unsigned, 1> &data){
	foo(std::get<0>(data));
	
    }

    std::function<void (unsigned&)> foo;
};


int exp() {
    /***************************************************************************
     * Configuration
     ****************************************************************************/

    // CommunicationPolicy
    typedef graybat::communicationPolicy::BMPI CP;
    
    // GraphPolicy
    typedef graybat::graphPolicy::BGL<Function>    GP;
    
    // Cage
    typedef graybat::Cage<CP, GP> Cage;
    typedef typename Cage::Event  Event;
    typedef typename Cage::Vertex Vertex;
    typedef typename Cage::Edge Edge;
    typedef typename Vertex::VertexProperty VertexProperty;

    /***************************************************************************
     * Initialize Communication
     ****************************************************************************/
    //const unsigned vertices = 10;
    
    // Create GoL Graph
    Cage cage;

    // Set communication pattern
    cage.setGraph(graybat::pattern::BidirectionalStar(cage.getPeers().size()));

    // Distribute vertices
    cage.distribute(graybat::mapping::Consecutive());
        

    /***************************************************************************
     * Run Simulation
     ****************************************************************************/
    //std::vector<Event> events;

    std::array<unsigned, 1> input {{0}};
    std::array<unsigned, 1> output {{0}};

    const Vertex reply = cage.getVertex(0);
    
    for(Vertex v : cage.hostedVertices){


        if(v == reply){
            while(true){
                Edge e = cage.recv(output);
                std::cout << "Got msg from " << e.source.id << ": "<< output[0] << std::endl;
                output[0]++;
                cage.send(e.inverse(), output);
            }
            
        }
        else {
            input[0] = v.id;
            v.spread(input);
            v.collect(input);
            std::cout << " Got input from master:" << input[0] << std::endl;
        }
 
	
    }

    return 0;

}

int main(){
    exp();
    return 0;
}
