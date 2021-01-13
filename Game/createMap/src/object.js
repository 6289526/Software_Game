class Object3D {
    /**
     * 
     * @param {*} scene
     * @param {string} geometryType 
     * @param {string} materialType 
     * @param {number} color
     * @param {number} width 
     * @param {number} height 
     * @param {number} depth 
     * @param {number} radius 
     * @param {number} segmentsW 
     * @param {number} segmentsH 
     * @param {string} imgPath
     */
    constructor(geometryType, materialType, color, width, height, depth, radius, segmentsW, segmentsH, imgPath) {
        this.material;
        switch (materialType) {
            case 'basic':
                this.material = new THREE.MeshBasicMaterial({ color: color });
                break;
            case 'normal':
                this.material = new THREE.MeshBasicMaterial({ color: color });
                break;
            case 'lambert':
                this.material = new THREE.MeshLambertMaterial({ color: color });
                break;
            case 'phong':
                this.material = new THREE.MeshPhongMaterial({ color: color });
                break;
            case 'toon':
                this.material = new THREE.MeshToonMaterial({ color: color });
                break;
            case 'standard':
                this.material = new THREE.MeshStandardMaterial({ color: color, roughness: 0.5 });
                break
            case 'texture':
                // 画像を読み込む
                const loader = new THREE.TextureLoader();
                const texture = loader.load(path);
                // マテリアルにテクスチャーを設定
                this.material = new THREE.MeshStandardMaterial({
                    map: texture
                });
                break
        }
        this.geometry;
        switch (geometryType) {
            case 'sphere':
                this.geometry = new THREE.SphereGeometry(radius, segmentsW, segmentsH);
                break;
            case 'box':
                this.geometry = new THREE.BoxGeometry(width, height, depth);
                break;
            case 'plane':
                this.collisionType = 'plane';
                this.geometry = new THREE.PlaneGeometry(width, height, segmentsW);
                this.material;
                switch (materialType) {
                    case 'basic':
                        this.material = new THREE.MeshBasicMaterial({ color: color, side: THREE.DoubleSide });
                        break;
                    case 'normal':
                        this.material = new THREE.MeshBasicMaterial({ color: color, side: THREE.DoubleSide });
                        break;
                    case 'lambert':
                        this.material = new THREE.MeshLambertMaterial({ color: color, side: THREE.DoubleSide });
                        break;
                    case 'phong':
                        this.material = new THREE.MeshPhongMaterial({ color: color, side: THREE.DoubleSide });
                        break;
                    case 'toon':
                        this.material = new THREE.MeshToonMaterial({ color: color, side: THREE.DoubleSide });
                        break;
                    case 'standard':
                        this.material = new THREE.MeshStandardMaterial({ color: color, roughness: 0.5, side: THREE.DoubleSide });
                        break
                    case 'texture':
                        // 画像を読み込む
                        const loader = new THREE.TextureLoader();
                        const texture = loader.load(path);
                        // マテリアルにテクスチャーを設定
                        this.material = new THREE.MeshStandardMaterial({
                            map: texture, side: THREE.DoubleSide
                        });
                        break
                }
                break;
            case 'cone':
                this.geometry = new THREE.ConeGeometry(radius, height, segmentsW, segmentsH);
                break;
            case 'cylinder':
                this.geometry = new THREE.CylinderGeometry(radius, radius, height, segmentsW, segmentsH);
                break;
            case 'torus':
                this.geometry = new THREE.TorusGeometry(radius, width, segmentsW, segmentsH);
                break;
        }


        this.mesh = new THREE.Mesh(this.geometry, this.material);
    }

    /**
     * 
     * @param {*} C 
     * @param {keyInput} inputData 
     */
    collision(C, inputData) {
        switch (this.collisionType) {
            case 'plane':
                if (onPlane(C, this.mesh) === true) {
                    return planeCollision(C, this.mesh, inputData);
                }else {
                    return willOnPlane(C, this.mesh, inputData);
                }
                break;
        }
    }

    SphereGeometry(radius, segmentsW, segmentsH) {
        this.geometry = new THREE.SphereGeometry(radius, segmentsW, segmentsH);
    }

    BoxGeometry(width, height, depth) {
        this.geometry = new THREE.BoxGeometry(width, height, depth);
    }

    PlaneGeometry(width, height, segmentsW, segmentsH) {
        this.geometry = new THREE.PlaneGeometry(width, height, segmentsW, segmentsH);
    }

    ConeGeometry(radius, height, segmentsW, segmentsH) {
        this.geometry = new THREE.ConeGeometry(radius, height, segmentsW, segmentsH);
    }

    CylinderGeometry(radius, height, segmentsW, segmentsH) {
        this.geometry = new THREE.CylinderGeometry(radius, radius, height, segmentsW, segmentsH);
    }

    TorusGeometry(radius, width, segmentsW, segmentsH) {
        this.geometry = new THREE.TorusGeometry(radius, width, segmentsW, segmentsH);
    }

}