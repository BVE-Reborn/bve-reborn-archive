using System;
using System.Data;
using System.IO;
using BVE.Resources;
using Native.bve.core.image;
using Native.bve.parsers.b3d_csv_object;
using TMPro;
using UnityEngine;
using UnityEngine.Assertions;
using UnityEngine.Experimental.Rendering;
using UnityEngine.Profiling;
using Debug = UnityEngine.Debug;
using File = System.IO.File;
using Mesh = UnityEngine.Mesh;

namespace BVE.ModelLoader {
    public class ModelLoadTest : MonoBehaviour {
        private MaterialManager _materialManager;

        private void LoadModel(string path) {
            if (!File.Exists(path)) {
                throw new ArgumentException($"Path {path} not found");
            }

            var ext = Path.GetExtension(path);
            if (ext != ".b3d" && ext != ".csv") {
                throw new ArgumentException($"File {path} is not a model file");
            }

            string contents;
            using (var file = File.OpenText(path)) {
                contents = file.ReadToEnd();
            }

            using (var obj = ParseObject(ext, contents)) {
                ProcessModel(path, obj);
            }
        }

        private ParsedB3DCSVObject ParseObject(string ext, string contents) {
            switch (ext) {
                case ".b3d":
                    return Native.bve_cs.parse_model_b3d(contents);
                case ".csv":
                    return Native.bve_cs.parse_model_csv(contents);
                default:
                    throw new ArgumentException($"Unknown Extenstion {ext}", nameof(ext));
            }
        }

        private void ProcessModel(string path, ParsedB3DCSVObject obj) {
            var errors = obj.errors;
            for (var i = 0; i < errors.Count; i++) {
                var error = errors[i];
                Debug.LogError($"Error in parsing {path}:{error.line} {error.error}");
            }

            var objMeshCount = obj.meshes.Count;
            var parentTransform = this.gameObject.transform;
            var metaGameObject = new GameObject($"{(uint) path.GetHashCode()}");
            metaGameObject.transform.parent = parentTransform;
            metaGameObject.transform.position = parentTransform.position;
            for (int meshIndex = 0; meshIndex < objMeshCount; meshIndex++) {
                var meshGameObject = new GameObject($"{(uint) path.GetHashCode()}:{meshIndex}");
                meshGameObject.transform.parent = metaGameObject.transform;
                meshGameObject.transform.position = metaGameObject.transform.position;
                var meshFilter = meshGameObject.AddComponent<MeshFilter>();
                var meshRenderer = meshGameObject.AddComponent<MeshRenderer>();

                var mesh = new Mesh();

                var objMesh = obj.meshes[meshIndex];
                var objVerts = objMesh.verts;
                var objIndices = objMesh.indices;

                var vertCount = objVerts.Count;
                var meshVerts = new Vector3[vertCount];
                var meshNormals = new Vector3[vertCount];
                var meshUV = new Vector2[vertCount];

                for (var i = 0; i < vertCount; i++) {
                    var vertData = objVerts[i];
                    meshVerts[i] = vertData.position.toVector3();
                    meshNormals[i] = vertData.normal.toVector3();
                    meshUV[i] = vertData.texture_coord.toVector2();
                }

                mesh.vertices = meshVerts;
                mesh.normals = meshNormals;
                mesh.uv = meshUV;

                var indicesCount = objIndices.Count;
                var meshIndices = new int[indicesCount];

                for (var i = 0; i < indicesCount; i++) {
                    meshIndices[i] = (int) objIndices[i];
                }

                mesh.triangles = meshIndices;

//                mesh.RecalculateNormals();
                mesh.RecalculateTangents();
                mesh.RecalculateBounds();
                mesh.Optimize();

                meshFilter.mesh = mesh;

                var objTexture = objMesh.texture;
                var texFullPath = ResolveTextureName(path, objTexture.file);
                var screenDoor = objTexture.has_transparent_color
                    ? (Color32?) objTexture.decal_transparent_color.toColor32()
                    : null;

                var mat = _materialManager.GetMaterial(texFullPath, screenDoor, objMesh.color.toColor32());

                meshRenderer.material = mat;
            }

            Debug.Log($"Loaded object {path}");
        }

        private static readonly string[] otherImageExtensions = new[] {".bmp", ".jpg", ".jpeg", ".png"};

        private static string CaseInsensitiveResolve(string file) {
            var fileAbs = Path.GetFullPath(file);
            var fileLower = fileAbs.ToLowerInvariant();

            var dir = Path.GetDirectoryName(fileAbs);
            var files = Directory.GetFiles(dir);
            var filesCount = files.Length;

            for (var i = 0; i < filesCount; i++) {
                var curFile = Path.GetFullPath(files[i]);
                var curFileLower = curFile.ToLowerInvariant();
                if (curFileLower == fileLower) {
                    return curFile;
                }

                for (var j = 0; j < otherImageExtensions.Length; j++) {
                    var curFileChanged = Path.ChangeExtension(curFileLower, otherImageExtensions[j]);
                    if (curFileChanged != fileLower)
                        continue;
                    Debug.Log($"Switched extension of {curFile} to {otherImageExtensions[j]}");
                    return curFile;
                }
            }

            return file;
        }

        private string ResolveTextureName(string objPath, string texturePath) {
            if (texturePath == "") {
                return texturePath;
            }

            if (Path.IsPathRooted(texturePath)) {
                return CaseInsensitiveResolve(texturePath);
            }

            var objRelative = CaseInsensitiveResolve(Path.Combine(objPath, "..", texturePath));
            if (File.Exists(objRelative)) {
                return Path.GetFullPath(objRelative);
            }

            var dataDir = Native.bve_cs.core_filesystem_data_directory().path();
            var dataRelative =
                CaseInsensitiveResolve(Path.Combine(dataDir, "LegacyContent/Railway/Object", texturePath));
            if (File.Exists(dataRelative)) {
                return Path.GetFullPath(dataRelative);
            }

            throw new ArgumentException(
                $"Unknown texture {texturePath}. Tried: \"{objRelative}\" and \"{dataRelative}\"");
        }

        public void Start() {
            _materialManager = GetComponent<MaterialManager>();

            LoadModel("E:/16) OpenBVE Files/LegacyContent/Train/R46 2014 (8 Car)/Cars/Body/BodyA.b3d");
        }

        // Update is called once per frame
        public void Update() { }
    }
}