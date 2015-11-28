// ============================================================
// 工程项目	:	MMORPG
// 文件名	:	EntityMaterialInstance.h
// 创建人	:	FreeKnight(王宏张)
// 说明		:	控制Entity的所有SubEntity透明度
// 创建时间	:	2007-8-24 12:38:01
// ============================================================
#ifndef __ENTITYMATERIALINSTANCE_H__
#define __ENTITYMATERIALINSTANCE_H__

#include "SubEntityMaterialInstance.h"

namespace Ogre
{
/** Iterator to traverse the SubEntityMaterialInstance's.
 * @author Kencho.
 */
typedef VectorIterator<std::vector<SubEntityMaterialInstance *> > SubEntityMaterialInstancesIterator;

/** An instance of a full Entity material.
 * This is like a hub for all the underlying SubEntities material instances.
 * It keeps a list of each SubEntity's material instance.
 * @see SubEntityMaterialInstance.
 * @author Kencho
 */
class OGREPRO_API EntityMaterialInstance
{
// Attributes =================================================================================
  public:
  protected:
    /** List of SubEntities' material instances.
     */
    std::vector<SubEntityMaterialInstance *> mSEMIs;
    /** Keeps the current transparency value.
     * @see SubEntityMaterialInstance::mCurrentTransparency.
     */
    Real mCurrentTransparency;
// Methods ====================================================================================
  public:
    /** Constructor. 
     * Initialises the list of SubEntities' material instances.
     * @param e The entity this material instance will affect to.
     * @note This will create material instances for all the underlying SubEntities.
     */
    EntityMaterialInstance (Entity *e = 0);
    /** Destructor.
     * Destroys all the underlying SubEntityMaterialInstances.
     */
    ~EntityMaterialInstance ();
    /** Assigns this material to all the SubEntities through their respective 
     * SubEntityMaterialInstances.
     * @param name Name of the new material for this entity (all of its SubEntities).
     * @see SubEntityMaterialInstance::setMaterialName().
     */
    void setMaterialName (String name);
    /** Sets the scene blending method for all the SubEntities.
     * @param sbt The desired SceneBlendType.
     * @see SubEntityMaterialInstance::setSceneBlending().
     */
    void setSceneBlending (SceneBlendType sbt);
    /** Changes the whole Entity transparency, through all the underlying SubEntityMaterialInstances.
     * @param transparency New transparency.
     * @see SubEntityMaterialInstance::setTransparency().
     */
    void setTransparency (Real transparency);
    /** Returns an iterator to traverse all the underlying MaterialInstances.
     * @return The SubEntityMaterialInstances iterator.
     */
    SubEntityMaterialInstancesIterator getSubEntityMaterialInstancesIterator ();
	
	// add by FreeKnight(王宏张) 2007-8-24
	// 清空数据
	void clear();

	// add by FreeKnight(王宏张) 2007-8-24
	// 设置模型实体
	void setEntity(Entity *e);
  protected:
};

};
#endif // __ENTITYMATERIALINSTANCE_H__
