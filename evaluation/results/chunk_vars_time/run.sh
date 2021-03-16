set -e

SCENE=$1

INTGR="path"
RUN="./../../scripts/start_eval.sh $INTGR $SCENE"
SOURCE=../../..
BUILD=$SOURCE/build/Evaluation

if [ ! -d $SCENE ]
then
    mkdir $SCENE
fi

if [ ! -d "output" ]
then
    mkdir output
fi

if ! [[ $* == *--skip-render* ]]; then
    COUNT_STATS="COUNT_STATS=False"

    BFSIZE="BF_SIZE=32"
    CHUNKSIZE="CHUNK_SIZE=64"
    cmake -S $SOURCE -B $BUILD -D$COUNT_STATS -D$BFSIZE -D$CHUNKSIZE
    make -C $BUILD -j
    FILENAME_POSTFIX="integer:$BFSIZE integer:$CHUNKSIZE"
    $RUN "octree-bfs" $FILENAME_POSTFIX
    $RUN "bvh-bfs" $FILENAME_POSTFIX

    BFSIZE="BF_SIZE=32"
    CHUNKSIZE="CHUNK_SIZE=128"
    cmake -S $SOURCE -B $BUILD $COUNT_STATS -D$BFSIZE -D$CHUNKSIZE
    make -C $BUILD -j
    FILENAME_POSTFIX="integer:$BFSIZE integer:$CHUNKSIZE"
    $RUN "octree-bfs" $FILENAME_POSTFIX
    $RUN "bvh-bfs" $FILENAME_POSTFIX

    BFSIZE="BF_SIZE=64"
    CHUNKSIZE="CHUNK_SIZE=64"
    cmake -S $SOURCE -B $BUILD $COUNT_STATS -D$BFSIZE -D$CHUNKSIZE
    make -C $BUILD -j
    FILENAME_POSTFIX="integer:$BFSIZE integer:$CHUNKSIZE"
    $RUN "octree-bfs" $FILENAME_POSTFIX
    $RUN "bvh-bfs" $FILENAME_POSTFIX

    BFSIZE="BF_SIZE=64"
    CHUNKSIZE="CHUNK_SIZE=128"
    cmake -S $SOURCE -B $BUILD $COUNT_STATS -D$BFSIZE -D$CHUNKSIZE
    make -C $BUILD -j
    FILENAME_POSTFIX="integer:$BFSIZE integer:$CHUNKSIZE"
    $RUN "octree-bfs" $FILENAME_POSTFIX
    $RUN "bvh-bfs" $FILENAME_POSTFIX
fi