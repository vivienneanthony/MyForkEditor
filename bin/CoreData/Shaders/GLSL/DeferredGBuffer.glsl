#if defined(PBR) && defined(DEFERRED)

    /// Convert an RGB color into YCoCg
    ///     color: input color to convert
    vec3 YCoCg_FromRGB(in vec3 color)
    {
        return vec3(
            0.25 * color.r + 0.5 * color.g + 0.25 * color.b, 
            0.5 * color.r - 0.5 * color.b + 0.5, 
            -0.25* color.r + 0.5 * color.g - 0.25 * color.b +0.5);
    }

    /// Convert a YCoCg encoded color into RGB
    ///     ycocg: The YCoCg encoded color to convert
    vec3 YCoCg_ToRGB(in vec3 ycocg)
    {
        ycocg.y -= 0.5;
        ycocg.z -= 0.5;
        return vec3(
            ycocg.r + ycocg.g - ycocg.b, 
            ycocg.r + ycocg.b, 
            ycocg.r - ycocg.g - ycocg.b);
    }
    
    /// Extract Albedo and specular components from interleaved
    ///     coords: screen coordinates of the interleaved pixel
    vec2 YCoCg_GetInterleaved(in vec2 coords)
    {
        return texture2D(sAlbedoBuffer, coords).ga;
    }
    
    /// Encodes the normal as Lambert Azimuth equal-area
    ///     normal: world space normal to pack
    ///     viewDir: viewing direction vector of the camera
    vec2 EncodeNormal(in vec3 normal, in vec3 viewDir)
    {    
        vec2 enc = normalize(normal.xy) * sqrt(-normal.z * 0.5 + 0.5);
        return enc * 0.5 + 0.5;
    }
   
    /// Decodes the normal from Lambert Azimuth equal-area packing
    ///     screenNormal: 2 channel packed normal from GBuffer
    ///     viewDir: viewing direction vector of the camera
    vec3 DecodeGBufferNormal(in vec2 screenNormal, in vec3 viewDir)
    {            
        vec4 nn = vec4(screenNormal, 0, 0) * vec4(2,2,0,0) + vec4(-1,-1,1,-1);
        float l = dot(nn.xyz,-nn.xyw);
        nn.z = l;
        nn.xy *= sqrt(l);
        return normalize(nn.xyz * 2.0 + vec3(0,0,-1));
    }

    /// Utility function for writing into the GBuffer, Places all GBuffer writes into a single place
    ///     screenPos: screen position of the pixel
    ///     albedo: albedo
    ///     specular: specular color
    ///     wsNormals: surface normal in worldspace
    ///     depth: depth position of the pixel
    ///     roughness: roughness of the surface
    void WriteGBuffer(in vec3 viewDir, in vec2 screenPos, in vec4 albedo, in vec3 specular, in vec3 wsNormals, in float roughness)
    {        
        // 2 channel normal
        gl_FragData[2].xy = EncodeNormal(wsNormals.xyz, viewDir);
        gl_FragData[2].z = roughness;
        gl_FragData[2].w = 1;
        
        // Interleave Co/Cg in a checkboard pattern        
        vec3 ycocgSpec = YCoCg_FromRGB(specular);
        vec3 ycocgAlbedo = YCoCg_FromRGB(albedo.rgb);
        
        screenPos = gl_FragCoord.xy;
        
        int x = int(screenPos.x);
        int y = int(screenPos.y);
        
        bool pattern = x % 2 == y % 2;
        gl_FragData[1].r = ycocgAlbedo.r;
        gl_FragData[1].g = pattern ? ycocgAlbedo.b : ycocgAlbedo.g;
        gl_FragData[1].b = ycocgSpec.r;
        gl_FragData[1].a = pattern ? ycocgSpec.b : ycocgSpec.g;
    }
    
    /// Extract YCoCg embedded abledo and specular
    ///     screenPos: position in the screen
    ///     coded: the encoded colors RGBA
    ///     abledo: output for decoded albedo
    ///     specColor: output for decoded specular color
    void DecodeYCoCgElements(in vec2 screenPos, in vec4 coded, out vec3 albedo, out vec3 specColor)
    {        
        vec2 interleaved = YCoCg_GetInterleaved(gl_FragCoord.xy * cGBufferInvSize);
        vec3 ycocgAlbedo = vec3(coded.rg, interleaved.x).rgb;
        vec3 ycocgSpecular = vec3(coded.ba, interleaved.y);
        
        screenPos = gl_FragCoord.xy;
        
        int x = int(screenPos.x);
        int y = int(screenPos.y);
       
        // swap channels if necessary
        bool pattern = x % 2 == y % 2;
        albedo = pattern ? ycocgAlbedo.rgb : ycocgAlbedo.rbg;
        specColor = pattern ? ycocgSpecular.rbg : ycocgSpecular.rgb;
        
        albedo = YCoCg_ToRGB(albedo);
        specColor = YCoCg_ToRGB(specColor);
    }
    
#endif