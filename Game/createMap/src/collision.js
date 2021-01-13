/**
 * 
 * @param {*} C 
 * @param {number} goX 
 * @param {number} goY 
 * @param {number} goZ 
 */
function collision_CB(C, goX, goY, goZ) {
    if (C.position.z < 0 || C.position.z > SQUARE_LENGTH * CELL_NUM_X) {
        console.log("マップの外z");
        return C.position;
    }
    if (C.position.y < -10) {
        console.log("マップの外y");
        return C.position;
    }
    if (C.position.x < 0 || C.position.x > SQUARE_LENGTH * CELL_NUM_Y) {
        console.log("マップの外x");
        return C.position;
    }
    if ((abs(goX) < 0.5 && abs(goY) < 0.5 && abs(goZ) < 0.5)) {

        console.log("もう無理", goX, goY, goZ);
        return C.position;
    }

    let cellX = floor(C.position.x / SQUARE_LENGTH);
    if (C.position.x - cellX * SQUARE_LENGTH >= 5) {
        cellX += 1;
    } else if (C.position.x - cellX * SQUARE_LENGTH <= -5) {
        cellX -= 1;
    }
    let goCellX = floor((C.position.x + goX) / SQUARE_LENGTH);
    if ((C.position.x + goX) - goCellX * SQUARE_LENGTH >= 5) {
        goCellX += 1;
    } else if ((C.position.x + goX) - goCellX * SQUARE_LENGTH <= -5) {
        goCellX -= 1;
    }
    let cellY = floor(C.position.y / (SQUARE_LENGTH));
    if (C.position.y - cellY * SQUARE_LENGTH >= 5) {
        cellY += 1;
    } else if (C.position.y - cellY * SQUARE_LENGTH <= -5) {
        cellY -= 1;
    }
    let goCellY = floor((C.position.y + goY) / (SQUARE_LENGTH));
    if ((C.position.y + goY) - goCellY * SQUARE_LENGTH >= 5) {
        goCellY += 1;
    }
    else if ((C.position.y + goY) - goCellY * SQUARE_LENGTH <= -5) {
        goCellY -= 1;
    }
    let cellZ = floor(C.position.z / SQUARE_LENGTH);
    if (C.position.z - cellZ * SQUARE_LENGTH >= 5) {
        cellZ += 1;
    } else if (C.position.z - cellZ * SQUARE_LENGTH <= -5) {
        cellZ -= 1;
    }
    let goCellZ = floor((C.position.z + goZ) / SQUARE_LENGTH);
    if ((C.position.z + goZ) - goCellZ * SQUARE_LENGTH >= 5) {
        goCellZ += 1;
    }
    else if ((C.position.z + goZ) - goCellZ * SQUARE_LENGTH <= -5) {
        goCellZ -= 1;
    }

    if (goCellX < 0 || goCellX > SQUARE_LENGTH * CELL_NUM_Y) {
        return collision_CB(C, goX / 2, goY, goZ);
    }
    if (goCellY < 0) {
        return collision_CB(C, goX, goY / 2, goZ);
    }
    if (goCellZ < 0 || goCellZ > SQUARE_LENGTH * CELL_NUM_X) {
        return collision_CB(C, goX, goY, goZ / 2);
    }
    console.log(cellX, cellY, cellZ);
    console.log(goCellX, goCellY, goCellZ);
    console.log(C.position);
    if (cellZ < goCellZ) {
        for (var i = cellZ; i <= goCellZ; i++) {
            if (cellX < goCellX) {
                for (var j = cellX; j <= goCellX; j++) {
                    if (cellY < goCellY) {
                        for (var k = cellY; k <= goCellY; k++) {
                            if (mapData[i][j][k] === 1) {
                                console.log("collision", i, j, k);
                                return collision_CB(C, goX / 2, goY / 2, goZ / 2);
                            }

                        }
                    }
                    else {
                        for (var k = goCellY; k <= cellY; k++) {
                            if (mapData[i][j][k] === 1) {
                                console.log("collision", i, j, k);
                                return collision_CB(C, goX / 2, goY / 2, goZ / 2);
                            }

                        }
                    }
                }
            }
            else {
                for (var j = goCellX; j <= cellX; j++) {
                    if (cellY < goCellY) {
                        for (var k = cellY; k <= goCellY; k++) {
                            if (mapData[i][j][k] === 1) {
                                console.log("collision", i, j, k);
                                return collision_CB(C, goX / 2, goY / 2, goZ / 2);
                            }

                        }
                    }
                    else {
                        for (var k = goCellY; k <= cellY; k++) {
                            if (mapData[i][j][k] === 1) {
                                console.log("collision", i, j, k);
                                return collision_CB(C, goX / 2, goY / 2, goZ / 2);
                            }

                        }
                    }
                }
            }
        }
    }
    else {
        for (var i = goCellZ; i <= cellZ; i++) {
            if (cellX < goCellX) {
                for (var j = cellX; j <= goCellX; j++) {
                    if (cellY < goCellY) {
                        for (var k = cellY; k <= goCellY; k++) {
                            if (mapData[i][j][k] === 1) {
                                console.log("collision", i, j, k);
                                return collision_CB(C, goX / 2, goY / 2, goZ / 2);
                            }

                        }
                    }
                    else {
                        for (var k = goCellY; k <= cellY; k++) {
                            if (mapData[i][j][k] === 1) {
                                console.log("collision", i, j, k);
                                return collision_CB(C, goX / 2, goY / 2, goZ / 2);
                            }

                        }
                    }
                }
            }
            else {
                for (var j = goCellX; j <= cellX; j++) {
                    if (cellY < goCellY) {
                        for (var k = cellY; k <= goCellY; k++) {
                            if (mapData[i][j][k] === 1) {
                                console.log("collision", i, j, k);
                                return collision_CB(C, goX / 2, goY / 2, goZ / 2);
                            }

                        }
                    }
                    else {
                        for (var k = goCellY; k <= cellY; k++) {
                            if (mapData[i][j][k] === 1) {
                                console.log("collision", i, j, k);
                                return collision_CB(C, goX / 2, goY / 2, goZ / 2);
                            }
                        }
                    }
                }
            }
        }
    }
    C.position.x += goX;
    if (C.position.x < 0) {
        C.position.x = 0;
    } else if (C.position.x > SQUARE_LENGTH * (CELL_NUM_Y - 1)) {
        C.position.x = SQUARE_LENGTH * (CELL_NUM_Y - 1);
    }
    C.position.y += goY;
    C.position.z += goZ;
    if (C.position.z < 0) {
        C.position.z = 0;
    } else if (C.position.z > SQUARE_LENGTH * (CELL_NUM_X - 1)) {
        C.position.z = SQUARE_LENGTH * (CELL_NUM_X - 1)
    }
    return C.position;
}

class myVector {
    constructor() {
        this.x = 0;
        this.y = 0;
        this.z = 0;
        this.scale;
    }
}

function vertVector(x1, y1, z1, x2, y2, z2) {
    var verticalVector = new myVector();
    verticalVector.x = y1 * z2 - z1 * y2;
    verticalVector.y = -(z1 * x2 - x1 * z2);
    verticalVector.z = x1 * y2 - y1 * x2;
    verticalVector.scale = sqrt(verticalVector.x * verticalVector.x + verticalVector.y * verticalVector.y + verticalVector.z * verticalVector.z);
    verticalVector.x /= verticalVector.scale;
    verticalVector.y /= verticalVector.scale;
    verticalVector.z /= verticalVector.scale;
    return verticalVector;
}

function nVertVector(x1, y1, z1, x2, y2, z2) {
    var verticalVector = new myVector();
    verticalVector.x = y1 * z2 - z1 * y2;
    verticalVector.y = -(z1 * x2 - x1 * z2);
    verticalVector.z = x1 * y2 - y1 * x2;
    verticalVector.scale = sqrt(verticalVector.x * verticalVector.x + verticalVector.y * verticalVector.y + verticalVector.z * verticalVector.z);
    
    return verticalVector;
}

function onPlane(C, plane) {
    // 左上の点から左下の点
    L1 = new myVector();
    L1.x = 0;
    L1.y = -4000 / sqrt(2);
    L1.z = -4000 / sqrt(2);
    L1C = new myVector();
    L1C.x = C.position.x - plane.position.x - 20;
    L1C.y = C.position.y - plane.position.y - 2000 / sqrt(2);
    L1C.z = C.position.z - plane.position.z - 2000 / sqrt(2);
    L1V = vertVector(L1.x, L1.y, L1.z, L1C.x, L1C.y, L1C.z);

    L2 = new myVector();
    L2.x = 40;
    L2.y = 0;
    L2.z = 0;
    L2C = new myVector();
    L2C.x = C.position.x - plane.position.x + 20;
    L2C.y = C.position.y - plane.position.y - 2000 / sqrt(2);
    L2C.z = C.position.z - plane.position.z - 2000 / sqrt(2);
    L2V = vertVector(L2.x, L2.y, L2.z, L2C.x, L2C.y, L2C.z);


    L3 = new myVector();
    L3.x = 0;
    L3.y = 4000 / sqrt(2);
    L3.z = 4000 / sqrt(2);
    L3C = new myVector();
    L3C.x = C.position.x - plane.position.x + 20;
    L3C.y = C.position.y - plane.position.y + 2000 / sqrt(2);
    L3C.z = C.position.z - plane.position.z + 2000 / sqrt(2);
    L3V = vertVector(L3.x, L3.y, L3.z, L3C.x, L3C.y, L3C.z);

    L4 = new myVector();
    L4.x = -40;
    L4.y = 0;
    L4.z = 0;
    L4C = new myVector();
    L4C.x = C.position.x - plane.position.x - 20;
    L4C.y = C.position.y - plane.position.y + 2000 / sqrt(2);
    L4C.z = C.position.z - plane.position.z + 2000 / sqrt(2);
    L4V = vertVector(L4.x, L4.y, L4.z, L4C.x, L4C.y, L4C.z);

    if (L1V.x >= 0 && L2V.x >= 0 && L3V.x >= 0 && L4V.x >= 0) {
        if (L1V.y >= 0 && L2V.y >= 0 && L3V.y >= 0 && L4V.y >= 0) {
            if (L1V.z >= 0 && L2V.z >= 0 && L3V.z >= 0 && L4V.z >= 0) {
                return true;
            } else if (L1V.z < 0 && L2V.z < 0 && L3V.z < 0 && L4V.z < 0) {
                return true;
            }
        }
        else if (L1V.y < 0 && L2V.y < 0 && L3V.y < 0 && L4V.y < 0) {
            if (L1V.z >= 0 && L2V.z >= 0 && L3V.z >= 0 && L4V.z >= 0) {
                return true;
            } else if (L1V.z < 0 && L2V.z < 0 && L3V.z < 0 && L4V.z < 0) {
                return true;
            }
        }
    }
    else if (L1V.x < 0 && L2V.x < 0 && L3V.x < 0 && L4V.x < 0) {
        if (L1V.y >= 0 && L2V.y >= 0 && L3V.y >= 0 && L4V.y >= 0) {
            if (L1V.z >= 0 && L2V.z >= 0 && L3V.z >= 0 && L4V.z >= 0) {
                return true;
            } else if (L1V.z < 0 && L2V.z < 0 && L3V.z < 0 && L4V.z < 0) {
                return true;
            }
        }
        else if (L1V.y < 0 && L2V.y < 0 && L3V.y < 0 && L4V.y < 0) {
            if (L1V.z >= 0 && L2V.z >= 0 && L3V.z >= 0 && L4V.z >= 0) {
                return true;
            } else if (L1V.z < 0 && L2V.z < 0 && L3V.z < 0 && L4V.z < 0) {
                return true;
            }
        }
    }
    console.log(L1V, L2V, L3V, L4V);
    console.log("上におらん")
    return false;

}

/**
 * 
 * @param {*} C 
 * @param {*} plane 
 * @param {keyInput} inputData 
 */
function planeCollision(C, plane, inputData) {
    x1 = 20;
    x2 = -20;
    y1 = (2000 / sqrt(2));
    y2 = (2000 / sqrt(2));
    z1 = (2000 / sqrt(2));
    z2 = (2000 / sqrt(2));
    v1 = vertVector(x1, y1, z1, x2, y2, z2);
    v2 = vertVector(x1, y1, z1, x2, y2, z2);
    if (`${v1}` === `${v2}`) {
        console.log("一緒")
    }
    C.position.x += inputData.goX;
    C.position.z += inputData.goZ;
    if (inputData.goY > 0) {
        C.position.y = plane.position.y + inputData.goY + ((-v1.x * (plane.position.x - C.position.x) - v1.z * (plane.position.z - C.position.z)) / v1.y);
    } else {
        C.position.y = plane.position.y + ((-v1.x * (plane.position.x - C.position.x) - v1.z * (plane.position.z - C.position.z)) / v1.y);
    }
    return C;
}

function willOnPlane(C, plane, inputData) {
    x1 = 20;
    x2 = -20;
    y1 = (2000 / sqrt(2));
    y2 = (2000 / sqrt(2));
    z1 = (2000 / sqrt(2));
    z2 = (2000 / sqrt(2));
    vn = nVertVector(x1, y1, z1, x2, y2, z2);
    v1 = new myVector();
    v1.x = C.position.x - plane.position.x;
    v1.y = C.position.y - plane.position.y;
    v1.z = C.position.z - plane.position.z;
    console.log((v1.x * vn.x + v1.y * vn.y + v1.z * vn.z) / (vn.scale));
    if ((v1.x * vn.x + v1.y * vn.y + v1.z * vn.z) / (vn.scale) > 0) {
        console.log("手前");
        if (abs((v1.x * vn.x + v1.y * vn.y + v1.z * vn.z) / (vn.scale)) < abs(inputData.goZ)) {
            console.log("すり抜ける")
            inputData.goZ = (v1.x * vn.x + v1.y * vn.y + v1.z * vn.z) / (vn.scale);
            
        }
        if (abs((v1.x * vn.x + v1.y * vn.y + v1.z * vn.z) / (vn.scale)) < sqrt(inputData.goY)){
            inputData.goY = (v1.x * vn.x + v1.y * vn.y + v1.z * vn.z) / (vn.scale);
        }
        
    } else {
        console.log("後ろ");
        if (abs((v1.x * vn.x + v1.y * vn.y + v1.z * vn.z) / (vn.scale)) < sqrt(inputData.goZ)) {
            console.log("すり抜ける")
            inputData.goZ = -1;
            
        }
        if (abs((v1.x * vn.x + v1.y * vn.y + v1.z * vn.z) / (vn.scale)) < sqrt(inputData.goY)){
            inputData.goY = -1;
        }
        
    }
    C.position.x += inputData.goX;
    C.position.y += inputData.goY;
    C.position.z += inputData.goZ;
    return C;

}
