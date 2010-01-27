#ifndef INCLUDE_HPP
#define INCLUDE_HPP

#include <Gosu/AutoLink.hpp>
#include <Gosu/Gosu.hpp>

#include <GL/glew.h>
#include <GL/glut.h>
#ifdef _MSC_VER
	#pragma comment(lib, "glew32.lib")
	#pragma comment(lib, "glut32.lib")
#endif

#include <vector>

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/lambda/lambda.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <math.h>

// meh
#ifdef _MSC_VER
	#include "../../../Gosu/fpsCounter/fpsCounter/fpsCounter.hpp"
	#include "../../../Gosu/Gosu-PostProcessing/Gosu-PostProcessing/Gosu-PostProcessing.hpp"
#else
	#include "fpsCounter.hpp"
	#include "Gosu-PostPRocessing.hpp"
#endif

#include "vecmat.hpp"

#include "gta2formats.hpp"
#include "filereader.hpp"
#include "style.hpp"
#include "vbo.hpp"
#include "block.hpp"
#include "map_part.hpp"
#include "map.hpp"


#endif