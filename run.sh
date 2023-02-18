SCENE=${1:-basic_scene}
FILENAME=${2:-image.png}

# Build
g++ main.c png.c ${SCENE}.c glad/src/glad.c -o main -Iglad/include/ -lOpenGL -lEGL -lpng -ldl

# Run
#./main $FILENAME

# Copy output image to html directory
#mv $FILENAME html/
