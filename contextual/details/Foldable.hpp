#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>

namespace Contextual::Details
{
  class Foldable
  {
    // askFold
    // askFoldMap
    // askFoldR
    // askFoldL
    // askNull
    // askElem
    // askToList
    // askLength
    // askElem
    // askMaximum
    // askMinimum
    // askSum
    // askProduct

    class Fold {
    };

    class FoldMap : public Static_curried<FoldMap, Nat<2>>{
    public:
      template<typename F, typename T>
    };


    //      -- | Combine the elements of a structure using a monoid.
//     fold :: Monoid m => t m -> m
//     fold = (m -> m) -> t m -> m
//
//     forall (t :: * -> *) m a. (Foldable t, Monoid m) => (a -> m) -> t a -> m
//     foldMap id

//     -- | Map each element of the structure to a monoid,
//     -- and combine the results.
//     foldMap :: Monoid m => (a -> m) -> t a -> m
//     {-# INLINE foldMap #-}
//     -- This INLINE allows more list functions to fuse. See #9848.
//     foldMap f = foldr (mappend . f) mempty

//     -- | A variant of 'foldMap' that is strict in the accumulator.
//     --
//     -- @since 4.13.0.0
//     foldMap' :: Monoid m => (a -> m) -> t a -> m
//     foldMap' f = foldl' (\ acc a -> acc <> f a) mempty

//     -- | Right-associative fold of a structure.
//     --
//     -- In the case of lists, 'foldr', when applied to a binary operator, a
//     -- starting value (typically the right-identity of the operator), and a
//     -- list, reduces the list using the binary operator, from right to left:
//     --
//     -- > foldr f z [x1, x2, ..., xn] == x1 `f` (x2 `f` ... (xn `f` z)...)
//     --
//     -- Note that, since the head of the resulting expression is produced by
//     -- an application of the operator to the first element of the list,
//     -- 'foldr' can produce a terminating expression from an infinite list.
//     --
//     -- For a general 'Foldable' structure this should be semantically identical
//     -- to,
//     --
//     -- @foldr f z = 'List.foldr' f z . 'toList'@
//     --
//     foldr :: (a -> b -> b) -> b -> t a -> b
//     foldr f z t = appEndo (foldMap (Endo #. f) t) z

//     -- | Right-associative fold of a structure, but with strict application of
//     -- the operator.
//     --
//     -- @since 4.6.0.0
//     foldr' :: (a -> b -> b) -> b -> t a -> b
//     foldr' f z0 xs = foldl f' id xs z0
//       where f' k x z = k $! f x z

//     -- | Left-associative fold of a structure.
//     --
//     -- In the case of lists, 'foldl', when applied to a binary
//     -- operator, a starting value (typically the left-identity of the operator),
//     -- and a list, reduces the list using the binary operator, from left to
//     -- right:
//     --
//     -- > foldl f z [x1, x2, ..., xn] == (...((z `f` x1) `f` x2) `f`...) `f` xn
//     --
//     -- Note that to produce the outermost application of the operator the
//     -- entire input list must be traversed. This means that 'foldl'' will
//     -- diverge if given an infinite list.
//     --
//     -- Also note that if you want an efficient left-fold, you probably want to
//     -- use 'foldl'' instead of 'foldl'. The reason for this is that latter does
//     -- not force the "inner" results (e.g. @z \`f\` x1@ in the above example)
//     -- before applying them to the operator (e.g. to @(\`f\` x2)@). This results
//     -- in a thunk chain \(\mathcal{O}(n)\) elements long, which then must be
//     -- evaluated from the outside-in.
//     --
//     -- For a general 'Foldable' structure this should be semantically identical
//     -- to,
//     --
//     -- @foldl f z = 'List.foldl' f z . 'toList'@
//     --
//     foldl :: (b -> a -> b) -> b -> t a -> b
//     foldl f z t = appEndo (getDual (foldMap (Dual . Endo . flip f) t)) z
//     -- There's no point mucking around with coercions here,
//     -- because flip forces us to build a new function anyway.

//     -- | Left-associative fold of a structure but with strict application of
//     -- the operator.
//     --
//     -- This ensures that each step of the fold is forced to weak head normal
//     -- form before being applied, avoiding the collection of thunks that would
//     -- otherwise occur. This is often what you want to strictly reduce a finite
//     -- list to a single, monolithic result (e.g. 'length').
//     --
//     -- For a general 'Foldable' structure this should be semantically identical
//     -- to,
//     --
//     -- @foldl' f z = 'List.foldl'' f z . 'toList'@
//     --
//     -- @since 4.6.0.0
//     foldl' :: (b -> a -> b) -> b -> t a -> b
//     foldl' f z0 xs = foldr f' id xs z0
//       where f' x k z = k $! f z x

//     -- | A variant of 'foldr' that has no base case,
//     -- and thus may only be applied to non-empty structures.
//     --
//     -- @'foldr1' f = 'List.foldr1' f . 'toList'@
//     foldr1 :: (a -> a -> a) -> t a -> a
//     foldr1 f xs = fromMaybe (errorWithoutStackTrace "foldr1: empty structure")
//                     (foldr mf Nothing xs)
//       where
//         mf x m = Just (case m of
//                          Nothing -> x
//                          Just y  -> f x y)

//     -- | A variant of 'foldl' that has no base case,
//     -- and thus may only be applied to non-empty structures.
//     --
//     -- @'foldl1' f = 'List.foldl1' f . 'toList'@
//     foldl1 :: (a -> a -> a) -> t a -> a
//     foldl1 f xs = fromMaybe (errorWithoutStackTrace "foldl1: empty structure")
//                     (foldl mf Nothing xs)
//       where
//         mf m y = Just (case m of
//                          Nothing -> y
//                          Just x  -> f x y)

//     -- | List of elements of a structure, from left to right.
//     --
//     -- @since 4.8.0.0
//     toList :: t a -> [a]
//     {-# INLINE toList #-}
//     toList t = build (\ c n -> foldr c n t)

//     -- | Test whether the structure is empty. The default implementation is
//     -- optimized for structures that are similar to cons-lists, because there
//     -- is no general way to do better.
//     --
//     -- @since 4.8.0.0
//     null :: t a -> Bool
//     null = foldr (\_ _ -> False) True

//     -- | Returns the size/length of a finite structure as an 'Int'.  The
//     -- default implementation is optimized for structures that are similar to
//     -- cons-lists, because there is no general way to do better.
//     --
//     -- @since 4.8.0.0
//     length :: t a -> Int
//     length = foldl' (\c _ -> c+1) 0

//     -- | Does the element occur in the structure?
//     --
//     -- @since 4.8.0.0
//     elem :: Eq a => a -> t a -> Bool
//     elem = any . (==)

//     -- | The largest element of a non-empty structure.
//     --
//     -- @since 4.8.0.0
//     maximum :: forall a . Ord a => t a -> a
//     maximum = fromMaybe (errorWithoutStackTrace "maximum: empty structure") .
//        getMax . foldMap (Max #. (Just :: a -> Maybe a))

//     -- | The least element of a non-empty structure.
//     --
//     -- @since 4.8.0.0
//     minimum :: forall a . Ord a => t a -> a
//     minimum = fromMaybe (errorWithoutStackTrace "minimum: empty structure") .
//        getMin . foldMap (Min #. (Just :: a -> Maybe a))

//     -- | The 'sum' function computes the sum of the numbers of a structure.
//     --
//     -- @since 4.8.0.0
//     sum :: Num a => t a -> a
//     sum = getSum #. foldMap Sum

//     -- | The 'product' function computes the product of the numbers of a
//     -- structure.
//     --
//     -- @since 4.8.0.0
//     product :: Num a => t a -> a
//     product = getProduct #. foldMap Product

// -- instances for Prelude types
  public:

    /**
     * @brief  Map each element of the structure to a monoid, and combine the results.
     */
    static constexpr FoldMap foldMap{};
  }; // end of class Foldable

} // end of namespace Contextual::Details
